/** @file
  This is a Shell CMOS access tool via ISA IO 0x70/0x71.

  Copyright (c) 2020 - 2015, ADLink technology. All rights reserved.<BR>
  
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>

//-#define PORT_72 0x72
//-#define PORT_73 0x73

UINTN PortIndex;
UINTN PortData;

/**
  Converts a union code character to upper case.
  This functions converts a unicode character to upper case.
  If the input Letter is not a lower-cased letter,
  the original value is returned.

  @param  Letter            The input unicode character.

  @return The upper cased letter.

**/
CHAR16
ToUpper (
  IN CHAR16                    Letter
  )
{
  if ('a' <= Letter && Letter <= 'z') {
    Letter = (CHAR16) (Letter - 0x20);
  }

  return Letter;
}

/**
  CMOS Wrtie
  
  @param[in] CmosOffset    Offset to write
  @param[in] CmosValue     Value to write

  @retval  Other           An error occurred.

**/
UINT8 CmosWrite( 
  UINT8 CmosOffset,
  UINT8 CmosValue
)
{
  IoWrite8 (PortIndex, CmosOffset);
  return IoWrite8 (PortData, CmosValue);
}

/**
  CMOS Read
  
  @param[in] CmosOffset    Offset to read

  @retval  CmosValue       Value to return.

**/
UINT8 CmosRead( 
  UINT8 CmosOffset
)
{
  IoWrite8 (PortIndex, CmosOffset);
  return IoRead8 (PortData);
}

/**
  Show Usage
  
  @retval  Other           An error occurred.

**/
VOID ShowHelp( VOID )
{
   Print (L"CMOS access usage: all arguments are unsigned integers in Hexdecimal.\n");
   Print (L"  Cmos -w CMOS_Offset Value\n");
   Print (L"  Cmos -r CMOS_Offset\n");
   Print (L"  Cmos -x -w Extend CMOS_Offset Value\n");
   Print (L"  Cmos -x -r Extended CMOS_Offset\n");
   Print (L"\n");
   return;
}

/**
  UEFI application entry point which has an interface similar to a
  standard C main function.

  The ShellCEntryLib library instance wrappers the actual UEFI application
  entry point and calls this ShellAppMain function.

  @param[in] Argc     The number of items in Argv.
  @param[in] Argv     Array of pointers to strings.

  @retval  0               The application exited normally.
  @retval  Other           An error occurred.

**/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  UINTN  i;
  UINT8  CmosValue;
  UINT8  CmosOffset;
  
  if (Argc == 1) {
    ShowHelp();
    return 0;
  }

  PortIndex = 0x70;
  PortData = 0x71;

  for (i = 1; i < Argc; i++) {
    if (Argv[i][0] == '-') {
      switch(ToUpper(Argv[i][1])) {
        case 'X':
          PortIndex = 0x72;
          PortData = 0x73;
          break;
        default:
          break;
      }
    }
  }
  
  for (i = 1; i < Argc; i++) {
    if (Argv[i][0] == '-') {
      switch(ToUpper(Argv[i][1])) {
        case 'W':
          CmosOffset = (UINT8)StrHexToUintn(Argv[++i]);
          CmosValue  = (UINT8)StrHexToUintn(Argv[++i]);
          CmosValue  = CmosWrite(CmosOffset, CmosValue);
          Print (L"Write 0x%02X to 0x%02X\n", CmosValue, CmosOffset);
          break;
        case 'R':
          CmosOffset = (UINT8)StrHexToUintn(Argv[++i]);
          CmosValue = CmosRead(CmosOffset);
          Print (L"Read 0x%02X from 0x%02X\n", CmosValue, CmosOffset);
          break;
        case '?':
        case 'H':
          ShowHelp();
          break;
        default:
          break;
      }
    } else {
      ShowHelp();
      break;
    }
  }

  return 0;
}

