/** @file
  This is a application ABS Diagnostic during next boot

  Copyright (c) 2020, . All rights reserved.<BR>
  
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Include/Uefi/UefiMultiPhase.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "AdlOsIndications.h"
#include "AdlGlobalVariable.h"

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
  EFI_STATUS     Status;
	EFI_GUID 	     AdlGlobalVariableGuid = ADL_GLOBAL_VARIABLE;
  UINT64         AdlOsIndicationsSupported;
  UINT64         AdlOsIndications;
  UINTN          DataSize;
  EFI_RESET_TYPE ResetType;
  
  //
  // Read AdlOsIndicationsSupported variable to determine if it is a Adlink OsIndications Supported environment
  //
  AdlOsIndications = 0;
  DataSize = sizeof(AdlOsIndicationsSupported);
  Status = gRT->GetVariable (
                  ADL_OS_INDICATIONS_SUPPORT_VARIABLE_NAME,
                  &AdlGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &AdlOsIndicationsSupported
                  );
	if(EFI_ERROR (Status) || (DataSize == 0)) {
    return Status;
  }
  //
  // Read AdlOsIndications variable and set ADL_OS_INDICATIONS_BOOT_TO_DIAG to boot to ABS Diagnotic menu
  //
  AdlOsIndications = 0;
  DataSize = sizeof(AdlOsIndications);
  Status = gRT->GetVariable (
                  ADL_OS_INDICATIONS_VARIABLE_NAME,
                  &AdlGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &AdlOsIndications
                  );

  AdlOsIndications |= ((UINT64) ADL_OS_INDICATIONS_BOOT_TO_DIAG);
  if (!EFI_ERROR (Status) || Status == EFI_NOT_FOUND) {
    Status = gRT->SetVariable (
                    ADL_OS_INDICATIONS_VARIABLE_NAME,
                    &AdlGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    sizeof (AdlOsIndications),
                    &AdlOsIndications
                    );
    if (!EFI_ERROR (Status)) {
      Print(L"AdlOsIndications successfully written, will boot to Setup Menu\n");
    } else {
      Print(L"Error writing AdlOsIndications - %r\n", Status);
    }
  } else {
    Print(L"Error reading AdlOsIndications - %r\n", Status);
  }
  //
  // Reboot to Firmware UI
  //
  if (Argc == 1) {
    ResetType = EfiResetWarm; // Default setting is Warm reset
  } else {
    if (*Argv[1] == '0' || *Argv[1] == '2' || *Argv[1] == '3') {
      ResetType = (EFI_RESET_TYPE)(*Argv[1] - '0'); // 0=EfiResetCold, 1=EfiResetWarm, 2=EfiResetShutdown, 3=EfiResetPlatformSpecific
    }
  }
  
  gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);

  return 0;
}
