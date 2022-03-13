#pragma once
#include <ntifs.h>

typedef struct _VMXCPUPCB 
{
	ULONG cpuNumber;
	PVOID VmxOnAddr;
	PHYSICAL_ADDRESS VmxOnAddrPhys;
}VMXCPUPCB,*PVMXCPUPCB;


PVMXCPUPCB VmxGetCPUPCB(ULONG cpuNumber);

PVMXCPUPCB VmxGetCurrentCPUPCB();


int VmxInit();

