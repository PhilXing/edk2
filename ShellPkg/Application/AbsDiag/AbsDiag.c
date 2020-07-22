/** @file
  This is a application ABS Diagnostic during next boot

  Copyright (c) 2020, . All rights reserved.<BR>
  
**/

#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "LaunchABSDiagApp.h"

/**
  UEFI application entry point which has an interface similar to a
  standard C main function.

  The ShellCEntryLib library instance wrappers the actual UEFI application
  entry point and calls this ShellAppMain function.

  @param[in] Argc     The number of items in Argv.
  @param[in] Argv     Array of pointers to strings.

  @retval  0               The application exited normally.
  @retval  Other           An error occurred.

  Below entry point assignment also do:
  
  in C:
    EFI_APPLICATION_ENTRY_POINT (ShellAppMain)
    
  in INF:
    [nmake.common]
      IMAGE_ENTRY_POINT=ShellAppMain
    
**/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
    EFI_STATUS Status;
    EFI_GUID LaunchABSDiagGuid = LAUNCH_ABSDIAG_GUID;
    LAUNCH_ABSDIAG_PROTOCOL 	*LaunchABSDiagProtocol = NULL;
    
    Status = gBS->LocateProtocol(&LaunchABSDiagGuid, NULL, &LaunchABSDiagProtocol);
    
    if (EFI_ERROR(Status)) {
      Print(L"Cannot locate LaunchABSDiag Protocol: Status=%r",Status);
    } else {
      Status = LaunchABSDiagProtocol->LaunchAppUI();
    }
    
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    
    return Status;
}
