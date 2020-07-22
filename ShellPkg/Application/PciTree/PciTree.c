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
UINT8 MaxBusNumber=0;
BOOLEAN bPrintAll=FALSE;
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

VOID
ScanBus(
//-  IN UINT8 ParentBusNumber,
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
  //
  // Quit enumerating devices if bus has been enumerated already
  //
  if (BusNumber < MaxBusNumber) {
    return;
  }
  //
  // Nested Device/Function loop
  //
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

      //
      // Next device if it's empty
      //
      if (HdrType01.Hdr.VendorId == 0xffff) {
        continue;
      }   
      if (IS_PCI_BRIDGE(&HdrType01) && (HdrType01.Bridge.PrimaryBus == BusNumber)) {
        //
        // Recursively enumerating on Bridge device on the same bus number
        //
        Print (L"   %03d   ",++DeviceCount);
        //
        // print bus level indicator
        //
        for (i=0; i<BusLevel; i++) {
          Print (L"-");
        }
        //
        // Print Bus, Device, Function
        //
        Print (L"B%02x.",BusNumber);
        Print (L"D%02x.",Device);
        Print (L"F%02x ",Func); 
        Print (L"(SB:%02x~",HdrType01.Bridge.SecondaryBus); 
        Print (L"%02x)\n",HdrType01.Bridge.SubordinateBus); 
        BusLevel++;
        for (Bus = HdrType01.Bridge.SecondaryBus; Bus <= HdrType01.Bridge.SubordinateBus; Bus++) {
          ScanBus(Bus);
        }
        BusLevel--;
        //
        // update maximum bus number assigned
        //
        MaxBusNumber=HdrType01.Bridge.SubordinateBus;
      } else {
        //
        // Print Bus, Device, Function if not a Bridge Devices
        //
        if (bPrintAll) {
          Print (L"   %03d   ",++DeviceCount);
          //
          // print bus level indicator
          //
          for (i=0; i<BusLevel; i++) {
            Print (L"-");
          }
          Print (L"B%02x.",BusNumber);
          Print (L"D%02x.",Device);
          Print (L"F%02x\n",Func); 
        }
      }
      //
      // next device if not a multi-function device
      //           
      if (!IS_PCI_MULTI_FUNC(&HdrType01)) {
        break; 
      }  
    } 
  }

}

/**
  Show Usage
  
  @retval  Other           An error occurred.

**/
VOID ShowHelp( VOID )
{
   Print (L"PciTree [-a]:\n");
   Print (L"  a: print all devices\n");
   Print (L"  print bridge devices by default\n");
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
  UINTN  i;

  for (i = 1; i < Argc; i++) {
    if (Argv[i][0] == '-') {
      switch(ToUpper(Argv[i][1])) {
        case 'A':
          bPrintAll=TRUE;
          break;
        default:
          ShowHelp();
          return 0;
      }
    } else {
      ShowHelp();
      return 0;
      break;
    }
  }

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &PciRootBridgeIo);
  if (EFI_ERROR (Status)) {
    gST->ConOut->OutputString (gST->ConOut, L"\nCan not load the PCI bridge IO protocol.");
    return Status;
  }

  if (bPrintAll) {
    Print (L"PCI Devices:\n");
  } else {
    Print (L"PCI Bridge Devices:\n");
  }
  ScanBus(0);

  return 0;
}
