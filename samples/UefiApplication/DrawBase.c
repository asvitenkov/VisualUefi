#include "DrawBase.h"
#include "console.h"

#include <Library/UefiLib.h>

//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>





EFI_GRAPHICS_OUTPUT_PROTOCOL* GetGraphicsOutputProtocol()
{
	EFI_STATUS efiStatus = EFI_SUCCESS;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = 0;

	efiStatus = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, &graphicsOuptutPtotocol);

	if (EFI_ERROR(efiStatus))
		Print(L"Failed to locate EFI graphics output protocol: %lx\n", efiStatus);

	return graphicsOuptutPtotocol;
}

EFI_STATUS SetPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOuptutPtotocol, UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	return graphicsOuptutPtotocol->Blt(graphicsOuptutPtotocol, &color, EfiBltVideoFill, 0, 0, x, y, 1, 1, 0);
}

EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* GetCurrentGraphicsModeInformation()
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = NULL;

	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = GetGraphicsOutputProtocol();

	if (graphicsOuptutPtotocol)
		info = graphicsOuptutPtotocol->Mode->Info;

	return info;
}

void DrawBresenhamLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol, UINTN x1, UINTN y1, UINTN x2, UINTN y2, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	EFI_STATUS efiStatus = EFI_SUCCESS;

	if (graphicsOuptutPtotocol)
	{
		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		efiStatus = SetPixel(graphicsOuptutPtotocol, x2, y2, color);

		if (EFI_ERROR(efiStatus))
		{
			Print(L"Failed to set pixel: %lx\n", efiStatus);
		}

		while (x1 != x2 || y1 != y2)
		{
			efiStatus = SetPixel(graphicsOuptutPtotocol, x1, y1, color);

			if (EFI_ERROR(efiStatus))
			{
				Print(L"Failed to set pixel: %lx\n", efiStatus);
			}
			else
			{
				const int error2 = error * 2;
				//
				if (error2 > -deltaY)
				{
					error -= deltaY;
					x1 += signX;
				}
				if (error2 < deltaX)
				{
					error += deltaX;
					y1 += signY;
				}
			}
		}
	}
}

void DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol, UINTN x1, UINTN y1, UINTN x2, UINTN y2, UINTN thickness, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	UINTN halfThickness = thickness / 2;

	if (abs(y1 - y2) < abs(x1 - x2))
	{
		for (int i = 0; i < thickness; i++)
		{
			DrawBresenhamLine(graphicsOuptutPtotocol, x1, (y1 + i) - halfThickness, x2, (y2 + i) - halfThickness, color);
		}
	}
	else
	{
		for (int i = 0; i < thickness; i++)
		{
			DrawBresenhamLine(graphicsOuptutPtotocol, (x1 - i) + halfThickness, y1, (x2 - i) + halfThickness, y2, color);
		}
	}
}

void DrawHorizontalLine(UINTN x, UINTN y, UINTN length, UINTN width, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	EFI_STATUS efiStatus = EFI_SUCCESS;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = GetGraphicsOutputProtocol();

	if (graphicsOuptutPtotocol)
	{
		efiStatus = graphicsOuptutPtotocol->Blt(graphicsOuptutPtotocol, &color, EfiBltVideoFill, 0, 0, x, y, length, width, 0);

		if (EFI_ERROR(efiStatus))
			Print(L"Failed to draw horizontal line: %lx\n", efiStatus);
	}
}

void DrawVerticalLine(UINTN x, UINTN y, UINTN length, UINTN width, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	EFI_STATUS efiStatus = EFI_SUCCESS;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = GetGraphicsOutputProtocol();

	if (graphicsOuptutPtotocol)
	{
		efiStatus = graphicsOuptutPtotocol->Blt(graphicsOuptutPtotocol, &color, EfiBltVideoFill, 0, 0, x, y, width, length, 0);

		if (EFI_ERROR(efiStatus))
			Print(L"Failed to draw horizontal line: %lx\n", efiStatus);
	}
}

void DrawRectangle(UINTN x, UINTN y, UINTN width, UINTN height, UINTN borderSize, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color)
{
	//ConsolePause();
	DrawHorizontalLine(x, y, width, borderSize, color);
	//ConsolePause();
	DrawHorizontalLine(x, y + height, width, borderSize, color);
	//ConsolePause();
	DrawVerticalLine(x, y, height, borderSize, color);
	//ConsolePause();
	DrawVerticalLine(x + width, y, height, borderSize, color);
	//ConsolePause();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void DrawBaseTest()
{
	{
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *currentModeInfo = GetCurrentGraphicsModeInformation();

		if (currentModeInfo)
		{
			EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = GetGraphicsOutputProtocol();

			if (graphicsOuptutPtotocol)
			{
				UINTN widthMax = currentModeInfo->HorizontalResolution;
				UINTN heightMax = currentModeInfo->VerticalResolution;
				UINTN borderSize = 1;
				//UINTN margin = 10;
				EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel = { 0, 0, 0, 0 };

				for (UINTN i = 0; i < heightMax / 2; ++i)
				{
					pixel.Blue = (UINT8)i * 1; pixel.Green = (UINT8)i * 3; pixel.Red = (UINT8)i * 2;
					DrawRectangle(i, i, widthMax - 2 * i, heightMax - 2 * i, borderSize, pixel);
				}

				EFI_GRAPHICS_OUTPUT_BLT_PIXEL redPixel = { 0, 0, 255, 0 };

				DrawLine(graphicsOuptutPtotocol, 10 + 100, 10 + 100, 210 + 100, 10 + 100, 10, redPixel);
				DrawLine(graphicsOuptutPtotocol, 210 + 100, 10 + 100, 210 + 100, 210 + 100, 10, redPixel);
				DrawLine(graphicsOuptutPtotocol, 210 + 100, 210 + 100, 10 + 100, 210 + 100, 10, redPixel);
				DrawLine(graphicsOuptutPtotocol, 10 + 100, 210 + 100, 10 + 100, 10 + 100, 10, redPixel);
				DrawLine(graphicsOuptutPtotocol, 10 + 100, 10 + 100, 210 + 100, 210 + 100, 10, redPixel);
				DrawLine(graphicsOuptutPtotocol, 10 + 100, 210 + 100, 210 + 100, 10 + 100, 10, redPixel);
			}
		}
	}
}