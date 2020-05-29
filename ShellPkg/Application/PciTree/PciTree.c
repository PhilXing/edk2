/** @file
  This is a test application that demonstrates how to travel thru whole PCI bus tree.

  Copyright (c) 2020, . All rights reserved.<BR>

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Include/Protocol/PciRootBridgeIo.h>
#include <Include/IndustryStandard/Pci22.h>

EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL          *PciRootBridgeIo;
UINTN DeviceCount=0;
UINTN BusLevel=0;

VOID
ScanBus(
  IN UINT8 BusNumber
  )
/*++

Routine Description:
  Scan a PCI bus to find a Bridge device to sacn deeper

Arguments:
  BusNumber: Bus number to start scanning

Returns:
  None
  
--*/
{
  UINT8                                    Bus;
  UINT8                                    Device;
  UINT8                                    Func;
  UINT64                                   Address;
  PCI_TYPE01                               HdrType01;
  UINTN i;

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    //
    // For each function, read its configuration space and print summary
    //
    for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
              
      Address = EFI_PCI_ADDRESS (BusNumber, Device, Func, 0); 
                        
      PciRootBridgeIo->Pci.Read (
                       PciRootBridgeIo, 
                       EfiPciWidthUint32,
                       Address, 
                       sizeof(HdrType01)/sizeof(UINT32), 
                       &HdrType01
                       ); 
             
      if (HdrType01.Hdr.VendorId == 0xffff) {
        continue;
      }   
           
      Print (L"   %03d   ",++DeviceCount);
      for (i=0; i<BusLevel; i++) {
        Print (L"-");
      }
      for (i=0; i<8-BusLevel; i++) {
        Print (L" ");
      }
      Print (L" %02x     ",BusNumber);
      Print (L" %02x     ",Device);
      Print (L" %02x     ",Func); 
      Print (L" %04x     ",HdrType01.Hdr.VendorId);
      Print (L" %04x\n"   ,HdrType01.Hdr.DeviceId);
      
      if (IS_PCI_BRIDGE(&HdrType01)) {
        for (Bus = HdrType01.Bridge.SecondaryBus; Bus <= HdrType01.Bridge.SubordinateBus; Bus++) {
          BusLevel++;
          ScanBus(Bus);
          BusLevel--;
        }
        
      }
                 
      if (!IS_PCI_MULTI_FUNC(&HdrType01)) {
        break; 
      }  
    } 
  }

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
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &PciRootBridgeIo);
  if (EFI_ERROR (Status)) {
    gST->ConOut->OutputString (gST->ConOut, L"\nCan not load the PCI bridge IO protocol.");
    return Status;
  }

  Print (L"             PCI Device List:\n\n");
  Print (L"          Bus     Dev     Func    VID     DID   \n");
  Print (L"          ---     ---     ----    ---     ---   \n");

  ScanBus(0);

  return 0;
}
