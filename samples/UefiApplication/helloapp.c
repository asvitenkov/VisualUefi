/*++

Copyright (c) Alex Ionescu.  All rights reserved.

Module Name:

    shvos.c

Abstract:

    This module implements the OS-facing UEFI stubs for SimpleVisor.

Author:

    Alex Ionescu (@aionescu) 29-Aug-2016 - Initial version

Environment:

    Kernel mode only.

--*/

//
// Basic UEFI Libraries
//
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>

//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//
// We run on any UEFI Specification
//
extern CONST UINT32 _gUefiDriverRevision = 0;

//
// Our name
//
CHAR8 *gEfiCallerBaseName = "ShellSample";

EFI_STATUS
EFIAPI
UefiUnload (
    IN EFI_HANDLE ImageHandle
    )
{
    // 
    // This code should be compiled out and never called 
    // 
    ASSERT(FALSE);
}

// TEST CODE

#include "DrawBase.h"
#include "console.h"


EFI_STATUS EFIAPI PrintAvaliableGraphicsModes()
{
	EFI_STATUS efiStatus = EFI_SUCCESS;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOuptutPtotocol = NULL;

	efiStatus = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, &graphicsOuptutPtotocol);

	if (EFI_ERROR(efiStatus))
	{
		Print(L"Failed to locate efi graphics output protocol: %lx\n", efiStatus);
		return efiStatus;
	}

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInformation = graphicsOuptutPtotocol->Mode->Info;
	UINT32 index = 0;
	UINTN size = 0;

	Print(L"Graphics modes count: %d\n", graphicsOuptutPtotocol->Mode->MaxMode);
	Print(L"Current mode: resolution %d x %d\n", modeInformation->HorizontalResolution, modeInformation->HorizontalResolution);

	for (index = 0; index < graphicsOuptutPtotocol->Mode->MaxMode; ++index)
	{
		efiStatus = graphicsOuptutPtotocol->QueryMode(graphicsOuptutPtotocol, index, &size, &modeInformation);

		if (!EFI_ERROR(efiStatus))
		{
			Print(L"mode %d: resolution %d x %d [press any key to continue]\n", index + 1, modeInformation->HorizontalResolution, modeInformation->VerticalResolution);
			ConsolePause();
		}
	}

	Print(L"[press any key to continue]\n");
	ConsolePause();

	return efiStatus;
}

EFI_STATUS EFIAPI PrintAvaliableTextModes()
{
	EFI_STATUS efiStatus = EFI_SUCCESS;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *textOutProtocol = gST->ConOut;
	UINTN cols, rows;

	Print(L"Console text modes count: %d\n", textOutProtocol->Mode->MaxMode);
	efiStatus = textOutProtocol->QueryMode(textOutProtocol, textOutProtocol->Mode->Mode, &cols, &rows);
	if (!EFI_ERROR(efiStatus))
		Print(L"Current text mode: %d x %d\n", cols, rows);

	for (UINTN i = 0; i < textOutProtocol->Mode->MaxMode; ++i)
	{
		efiStatus = textOutProtocol->QueryMode(textOutProtocol, i, &cols, &rows);

		if (!EFI_ERROR(efiStatus))
		{
			Print(L"mode %d: %d x %d [press any key to continue]\n", i + 1, cols, rows);
			ConsolePause();
		}
	}

	Print(L"[press any key to continue]\n");
	ConsolePause();

	return efiStatus;
}

void ResetTextOutputDevice()
{
	gST->ConOut->Reset(gST->ConOut, FALSE);
}

EFI_STATUS
EFIAPI
UefiMain (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE* SystemTable
    )
{
	ResetTextOutputDevice();
	DrawBaseTest();

	/*

	Print(L"Hello World! My handle is %lx and System Table is at %p\n",
	    ImageHandle, SystemTable);

	ResetTextOutputDevice();
	PrintAvaliableGraphicsModes();
	PrintAvaliableTextModes();
	ResetTextOutputDevice();
	*/


	Print(L"Exit UEFI application\npress any key to exit...");
	ConsolePause();

    return EFI_SUCCESS;
}

