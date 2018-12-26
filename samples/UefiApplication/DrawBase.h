#pragma once

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

void DrawHorizontalLine(UINTN x, UINTN y, UINTN length, UINTN width, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);

void DrawVerticalLine(UINTN x, UINTN y, UINTN length, UINTN width, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);

void DrawRectangle(UINTN x, UINTN y, UINTN width, UINTN height, UINTN borderSize, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);


void DrawBaseTest();

