#include<ntifs.h> 
#include <ntddk.h>
#include <ntstrsafe.h>

VOID DRIVERUNLOAD(_In_ struct _DRIVER_OBJECT* DriverObject)
{
	KdPrintEx((77, 0, "unload\r\n"));
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg)
{
	KdPrintEx((77, 0, "entry\r\n"));


	pDriver->DriverUnload = DRIVERUNLOAD;
	return STATUS_SUCCESS;
}