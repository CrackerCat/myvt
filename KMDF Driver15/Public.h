/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFDriver15,
    0x2a19fc02,0x60a5,0x4a35,0xb7,0x04,0x08,0x54,0x00,0x62,0x02,0x44);
// {2a19fc02-60a5-4a35-b704-085400620244}
