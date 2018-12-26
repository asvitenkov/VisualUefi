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

EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* GetCurrentGraphicsModeInformation()
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = NULL;

	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = GetGraphicsOutputProtocol();

	if (graphicsOuptutPtotocol)
		info = graphicsOuptutPtotocol->Mode->Info;

	return info;
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
			UINTN widthMax = currentModeInfo->HorizontalResolution;
			UINTN heightMax = currentModeInfo->VerticalResolution;
			UINTN borderSize = 1;
			//UINTN margin = 10;
			EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel = { 0, 0, 0, 0 };

			for (UINTN i = 0; i < heightMax / 2; ++i)
			{
				pixel.Blue = (UINT8)i * 1; pixel.Green = (UINT8)i*3; pixel.Red = (UINT8)i*2;
				DrawRectangle(i, i, widthMax - 2 * i, heightMax - 2 * i, borderSize, pixel);
			}
		}
	}
}