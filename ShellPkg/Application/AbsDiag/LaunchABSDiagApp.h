//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
//-----------------------------------------------------------------------------
//
//              Copyright (c) 2014 - 2016 ADLINK Technology Inc.
//                            ALL RIGHTS RESERVED
//
//-----------------------------------------------------------------------------
//                      R E V I S I O N    H I S T O R Y
//-----------------------------------------------------------------------------
//  Rev Date       Name       Description
//  --- ---------- ---------- -------------------------------------------------
//  003 05/26/2016 R. Chao    Change Copyright to 2016.
//  002 11/10/2015 R. Chao    Modify it to be module part.
//  001 10/03/2014 A. Chu     Initial revision.

/**
 * @file
 * LaunchDiagApp private header file
 */
#ifndef __LAUNCH_ABSDIAG_APP_H_
#define __LAUNCH_ABSDIAG_APP_H_

#include <Uefi.h>
/**
 * Structure that defines an internal structure used to maintain file and directory lists
 */

/**
 * GUID definition for the LAUNCH_ABSDIAG_GUID
 */
#define LAUNCH_ABSDIAG_GUID \
    {0xac8f4b08, 0xbf2a, 0x455e, 0xab, 0x2f, 0xcc, 0x47, 0x74, 0x4c, 0xab, 0x62}

/**
 * Function pointer declaration for the Lunch ABSDiag Protocol
 */
typedef EFI_STATUS(*LAUNCH_ABSDIAG)(VOID);

/**
 * Protocol definition structure for the Launch ABSDiag protocol
 */
typedef struct _LAUNCH_ABSDIAG_PROTOCOL
{
    LAUNCH_ABSDIAG LaunchAppUI;  ///< Pointer to the main function of the LaunchABSDiayByHotkey
} LAUNCH_ABSDIAG_PROTOCOL;

#endif

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
