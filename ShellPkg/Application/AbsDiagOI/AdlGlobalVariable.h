/** @file
  GUID for EFI (NVRAM) Variables.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

  @par Revision Reference:
  GUID defined in UEFI 2.1
**/

#ifndef __ADL_GLOBAL_VARIABLE_GUID_H__
#define __ADL_GLOBAL_VARIABLE_GUID_H__

#define ADL_GLOBAL_VARIABLE \
  { \
    0xd63c17f9, 0xb46b, 0x4655, {0xad, 0xda, 0xb7, 0x75, 0xfe, 0xd7, 0x2c, 0x84 } \
  }

//-extern ADL_GUID gAdlGlobalVariableGuid;

//
// Follow UEFI 2.4 spec:
// To prevent name collisions with possible future globally defined variables,
// other internal firmware data variables that are not defined here must be
// saved with a unique VendorGuid other than ADL_GLOBAL_VARIABLE or
// any other GUID defined by the UEFI Specification. Implementations must
// only permit the creation of variables with a UEFI Specification-defined
// VendorGuid when these variables are documented in the UEFI Specification.
//
///
/// Allows the firmware to indicate supported features and actions to the OS.
/// Its attribute is BS+RT.
///
#define ADL_OS_INDICATIONS_SUPPORT_VARIABLE_NAME    L"AdlOsIndicationsSupported"
///
/// Allows the OS to request the firmware to enable certain features and to take certain actions.
/// Its attribute is NV+BS+RT.
///
#define ADL_OS_INDICATIONS_VARIABLE_NAME            L"AdlOsIndications"

#endif
