/** @file
  This is a application that demonstrates how to enter firmware setup during next boot

  Copyright (c) 2020, . All rights reserved.<BR>
  
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/ShellCEntryLib.h>
#include <Include\Uefi\UefiMultiPhase.h>
#include <Library/UefiRuntimeServicesTableLib.h>

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
  UINT64         OsIndicationsSupported;
  UINT64         OsIndications;
  UINTN          DataSize;
  EFI_RESET_TYPE ResetType;
  
  //
  // Read OsIndicationsSupported variable to determine if it is a UEFI OS
  //
  OsIndications = 0;
  DataSize = sizeof(OsIndicationsSupported);
  Status = gRT->GetVariable (
                  L"OsIndicationsSupported",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &OsIndicationsSupported
                  );
	if(EFI_ERROR (Status) || (DataSize == 0)) {
    return Status;
  }
  //
  // Read OsIndications variable and set EFI_OS_INDICATIONS_BOOT_TO_FW_UI to boot to BIOS setup menu
  //
  OsIndications = 0;
  DataSize = sizeof(OsIndications);
  Status = gRT->GetVariable (
                  L"OsIndications",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &OsIndications
                  );

  OsIndications |= ((UINT64) EFI_OS_INDICATIONS_BOOT_TO_FW_UI);
  if (!EFI_ERROR (Status) || Status == EFI_NOT_FOUND) {
    Status = gRT->SetVariable (
                    L"OsIndications",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    sizeof (OsIndications),
                    &OsIndications
                    );
    if (!EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OsIndications successfully written, will boot to Setup Menu\n"));
    } else {
      DEBUG ((DEBUG_INFO, "Error writing OsIndications - %r\n", Status));
    }
  } else {
    DEBUG ((DEBUG_INFO, "Error reading OsIndications - %r\n", Status));
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
  
  gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);

  return 0;
}
