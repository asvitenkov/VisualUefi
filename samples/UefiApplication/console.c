#include "console.h"

#include <Library/UefiLib.h>

//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

void EFIAPI ConsolePause()
{
	UINTN EventIndex;
	gST->ConIn->Reset(gST->ConIn, FALSE);
	gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
}