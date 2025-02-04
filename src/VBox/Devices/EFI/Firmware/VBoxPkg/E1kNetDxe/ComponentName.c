/** @file

  Component Name code for the virtio-net driver.

  Copyright (c) 2021 - 2024, Oracle and/or its affiliates.
  Copyright (C) 2013, Red Hat, Inc.
  Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiLib.h>

#include "E1kNet.h"

STATIC
EFI_UNICODE_STRING_TABLE mE1kNetDriverNameTable[] = {
  { "eng;en", L"E1000 network interface card Driver" },
  { NULL,     NULL                                   }
};

STATIC
EFI_UNICODE_STRING_TABLE mE1kNetControllerNameTable[] = {
  { "eng;en", L"E1000 network interface card Driver" },
  { NULL,     NULL                                   }
};

/**
  Retrieves a Unicode string that is the user-readable name of the EFI Driver.

  @param  This       A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param  Language   A pointer to a three-character ISO 639-2 language
                     identifier. This is the language of the driver name that
                     that the caller is requesting, and it must match one of
                     the languages specified in SupportedLanguages.  The number
                     of languages supported by a driver is up to the driver
                     writer.
  @param  DriverName A pointer to the Unicode string to return.  This Unicode
                     string is the name of the driver specified by This in the
                     language specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER DriverName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/

STATIC
EFI_STATUS
EFIAPI
E1kNetGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL *This,
  IN  CHAR8                       *Language,
  OUT CHAR16                      **DriverName
  )
{
  return (Language == NULL || DriverName == NULL) ?
         EFI_INVALID_PARAMETER :
         LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mE1kNetDriverNameTable,
           DriverName,
           (BOOLEAN) (This == &gE1kNetComponentName) // Iso639Language
           );
}


/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by an EFI Driver.

  @param  This             A pointer to the EFI_COMPONENT_NAME_PROTOCOL
                           instance.
  @param  ControllerHandle The handle of a controller that the driver specified
                           by This is managing.  This handle specifies the
                           controller whose name is to be returned.
  @param  ChildHandle      The handle of the child controller to retrieve the
                           name of.  This is an optional parameter that may be
                           NULL.  It will be NULL for device drivers.  It will
                           also be NULL for a bus drivers that wish to retrieve
                           the name of the bus controller.  It will not be NULL
                           for a bus driver that wishes to retrieve the name of
                           a child controller.
  @param  Language         A pointer to a three character ISO 639-2 language
                           identifier.  This is the language of the controller
                           name that the caller is requesting, and it must
                           match one of the languages specified in
                           SupportedLanguages.  The number of languages
                           supported by a driver is up to the driver writer.
  @param  ControllerName   A pointer to the Unicode string to return.  This
                           Unicode string is the name of the controller
                           specified by ControllerHandle and ChildHandle in the
                           language specified by Language, from the point of
                           view of the driver specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user-readable name
                                in the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.
  @retval EFI_INVALID_PARAMETER ControllerHandle is NULL.
  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER ControllerName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/

STATIC
EFI_STATUS
EFIAPI
E1kNetGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN  EFI_HANDLE                  ChildHandle,
  IN  CHAR8                       *Language,
  OUT CHAR16                      **ControllerName
  )
{
  EFI_STATUS Status;

  if (ControllerHandle == NULL || Language == NULL || ControllerName == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // confirm that the device is managed by this driver, using the PciIo
  // Protocol
  //
  Status = EfiTestManagedDevice (
             ControllerHandle,
             gE1kNetDriverBinding.DriverBindingHandle,
             &gEfiPciIoProtocolGuid
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // we don't give different names to the bus (= parent) handle and the
  // child (= MAC) handle
  //
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mE1kNetControllerNameTable,
           ControllerName,
           (BOOLEAN) (This == &gE1kNetComponentName) // Iso639Language
           );
}

EFI_COMPONENT_NAME_PROTOCOL gE1kNetComponentName = {
  &E1kNetGetDriverName,
  &E1kNetGetControllerName,
  "eng" // SupportedLanguages, ISO 639-2 language codes
};

EFI_COMPONENT_NAME2_PROTOCOL gE1kNetComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME)     &E1kNetGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) &E1kNetGetControllerName,
  "en" // SupportedLanguages, RFC 4646 language codes
};
