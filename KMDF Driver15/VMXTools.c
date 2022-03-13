#include "VMXTools.h"
#include "VMXDefine.h"
#include <intrin.h>

//���Bios�Ƿ���VT
BOOLEAN VmxIsCheckSupportVTBIOS()
{
	ULONG64 value = __readmsr(IA32_FEATURE_CONTROL);

	return (value & 0x5) == 0x5;
}


//���CPU�Ƿ�֧��VT
BOOLEAN VmxIsCheckSupportVTCPUID()
{
	int cpuidinfo[4];
	__cpuidex(cpuidinfo, 1, 0);
	//CPUID �Ƿ�֧��VT ecx.vmx��6λ ���Ϊ1��֧��VT������֧��
	return (cpuidinfo[2] >> 5) & 1;
}


//���CR4VT�Ƿ��������Ϊ1 �����Ѿ��������ˣ�����û�п���
BOOLEAN VmxIsCheckSupportVTCr4()
{
	ULONG64 mcr4 = __readcr4();
	//���CR4 VT�Ƿ�����cr4.vmxe�����14λΪ1����ôVT�Ѿ���������������Կ���
	return ((mcr4 >> 13) & 1) == 0;
}