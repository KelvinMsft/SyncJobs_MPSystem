extern "C"
{ 
	#include <fltKernel.h>  
	#include "PauseCPU.h"
	#include "Util.h"

	//-----------------------------------------------------------------------------------------
	VOID DriverUnload(
			_In_ struct _DRIVER_OBJECT *DriverObject
		)
	{
		UNREFERENCED_PARAMETER(DriverObject);
		APIC_IPI_DEBUG_INFO("Unloaded Driver");
	}

	//---------------------------------------------------------------------------------------------------------------
	_IRQL_requires_min_(IPI_LEVEL) ULONG __stdcall TestDemo(
		PVOID params
	)
	{
		UNREFERENCED_PARAMETER(params);

		int i = 0;
		while (i < 10)
		{
			APIC_IPI_DEBUG_INFO("We dun Suspend Id: %x i: %d \r\n", KeGetCurrentProcessorNumberEx(nullptr), i);
			i++;
		}
		return 0;
	}

	//-----------------------------------------------------------------------------------------
	NTSTATUS DriverEntry(
		PDRIVER_OBJECT driver_object, 
		PUNICODE_STRING unicode
	)
	{
		NTSTATUS status = STATUS_SUCCESS; 
		UNREFERENCED_PARAMETER(unicode);
	 
		driver_object->DriverUnload = DriverUnload;

		APIC_IPI_COMMON_DBG_BREAK();

		InitSyncProcedure(TestDemo);

		return status;
	} 
}
