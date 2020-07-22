/** @file
  This is a application ABS Diagnostic during next boot

  Copyright (c) 2020, . All rights reserved.<BR>
  
**/

#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define B_PCH_PMC_GEN_PMCON_PWROK_FLR             BIT16 // PWROK Failure
#define B_PCH_PMC_GEN_PMCON_SUS_PWR_FLR           BIT14 // SUS Well Power Failure
#define PMC_BASE_ADDRESS                  0xFED03000 // PMC Memory Base Address
#define R_PCH_PMC_GEN_PMCON_1                     0x20  // General PM Configuration 1


/**
    Get South Bridge Miscellaneous BIT Status

    @param SbMiscType   Please check SB_MISC_TYPE for the details
    @param bPowerOkFailure    The value of the Miscellaneous BIT

    @retval EFI_UNSUPPORTED   This Miscellaneous BIT is not supported
    @retval EFI_SUCCESS       Success to get the value of the Miscellaneous BIT

**/
EFI_STATUS SbGetPwrokStatus( 
    IN OUT BOOLEAN            *bPowerOkFailure
)
{
    UINT32      Data32;

    Data32 = MmioRead32(PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1);
    
        if (Data32 & (UINT32) B_PCH_PMC_GEN_PMCON_PWROK_FLR) {
          *bPowerOkFailure = TRUE;
        } else {
          *bPowerOkFailure = FALSE;
        }
        return EFI_SUCCESS;
}

EFI_STATUS SbGetSusPwrokStatus( 
    IN OUT BOOLEAN            *bSusPowerFailure
)
{
    UINT32      Data32;

    Data32 = MmioRead32(PMC_BASE_ADDRESS + R_PCH_PMC_GEN_PMCON_1);
    
        if (Data32 & (UINT32) B_PCH_PMC_GEN_PMCON_SUS_PWR_FLR) {
          *bSusPowerFailure = TRUE;
        } else {
          *bSusPowerFailure = FALSE;
        }
        return EFI_SUCCESS;
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
    EFI_STATUS  Status;
    BOOLEAN     bPowerOkFailure;
    BOOLEAN     bSusPowerFailure;
    
    Status = SbGetPwrokStatus(&bPowerOkFailure);
 
    if (bPowerOkFailure) {
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    } else {
      Print(L"It's not a G3 power up.\n");
    }

    Status = SbGetSusPwrokStatus(&bSusPowerFailure);
   
    if (bSusPowerFailure) {
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    } else {
      Print(L"It's not a Sus Power Failure.\n");
    }
    
    return Status;
}

