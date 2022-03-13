#include "vmx.h"
#include <intrin.h>
#include "VMXDefine.h"

VMXCPUPCB vmxCpuPcbs[128];

PVMXCPUPCB VmxGetCPUPCB(ULONG cpuNumber)
{
	return &vmxCpuPcbs[cpuNumber];
}

PVMXCPUPCB VmxGetCurrentCPUPCB()
{
	return VmxGetCPUPCB(KeGetCurrentProcessorNumberEx(NULL));
}

int VmxInitVmOn()
{
	PVMXCPUPCB pVcpu = VmxGetCurrentCPUPCB();
	
	PHYSICAL_ADDRESS lowphys,heiPhy;
	
	lowphys.QuadPart = 0;
	heiPhy.QuadPart = -1;

	pVcpu->VmxOnAddr = MmAllocateContiguousMemorySpecifyCache(PAGE_SIZE, lowphys, heiPhy, lowphys, MmCached);

	if (!pVcpu->VmxOnAddr)
	{
		//申请内存失败
		return -1;
	}

	memset(pVcpu->VmxOnAddr, 0, PAGE_SIZE);

	pVcpu->VmxOnAddrPhys = MmGetPhysicalAddress(pVcpu->VmxOnAddr);

	//填充ID
	ULONG64 vmxBasic = __readmsr(IA32_VMX_BASIC);

	*(PULONG)pVcpu->VmxOnAddr = (ULONG)vmxBasic;

	//CR0,CR4

	ULONG64 vcr00 = __readmsr(IA32_VMX_CR0_FIXED0);
	ULONG64 vcr01 = __readmsr(IA32_VMX_CR0_FIXED1);

	ULONG64 vcr04 = __readmsr(IA32_VMX_CR4_FIXED0);
	ULONG64 vcr14 = __readmsr(IA32_VMX_CR4_FIXED1);

	ULONG64 mcr4 = __readcr4();
	ULONG64 mcr0 = __readcr0();

	mcr4 |= vcr04;
	mcr4 &= vcr14;

	mcr0 |= vcr00;
	mcr0 &= vcr01;

	//
	__writecr0(mcr0);

	__writecr4(mcr4);

	int error = __vmx_on(&pVcpu->VmxOnAddrPhys.QuadPart);

	if (error)
	{
		//释放内存，重置CR4
		mcr4 &= ~vcr04;
		__writecr4(mcr4);
		MmFreeContiguousMemorySpecifyCache(pVcpu->VmxOnAddr, PAGE_SIZE, MmCached);
		pVcpu->VmxOnAddr = NULL;
		pVcpu->VmxOnAddrPhys.QuadPart = 0;
	}

	return error;
}

int VmxInit()
{

	PVMXCPUPCB pVcpu = VmxGetCurrentCPUPCB();

	pVcpu->cpuNumber = KeGetCurrentProcessorNumberEx(NULL);

	int error = VmxInitVmOn();

	if (error)
	{
		DbgPrintEx(77, 0, "[db]:vmon 初始化失败 error = %d,cpunumber %d\r\n", error, pVcpu->cpuNumber);


		return error;
	}



	return 0;
}

