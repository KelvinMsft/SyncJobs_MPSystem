extern "C"
{
#include "Util.h"
#include "PauseCPU.h"

	//-----------------------------------------------------------------------------------------
	ULONG_PTR BroadCaster(
		_In_ ULONG_PTR Argument
	)
	{
		MCONTEXT* context = (MCONTEXT*)Argument;
	
		//other CPU
		if (KeGetCurrentProcessorNumberEx(nullptr) != context->ProcessorId)
		{ 
			APIC_IPI_DEBUG_INFO("Core: %d is Waiting...  \r\n", KeGetCurrentProcessorNumberEx(nullptr));
			
			InterlockedDecrement(&context->RunningProcessor); 
			
			while (context->Done == FALSE)
			{ 
				YieldProcessor();
			}

			InterlockedDecrement(&context->ProcessorsToResume); 

			APIC_IPI_DEBUG_INFO("Core %d is Resuming... \r\n", KeGetCurrentProcessorNumberEx(nullptr));
			 
		}	
		//current CPU (a CPU we want it works)
		else  
		{
			while (context->RunningProcessor != 0)
			{
				YieldProcessor();
			}

			APIC_IPI_DEBUG_INFO("----- start sync jobs by Core: %d ----- \r\n", KeGetCurrentProcessorNumberEx(nullptr));
			if (context->callback)
			{
				context->callback(context->Params);
			}
			APIC_IPI_DEBUG_INFO("--------- finshed by Core: %d --------- \r\n", KeGetCurrentProcessorNumberEx(nullptr));

			context->Done = TRUE; 

			while (context->ProcessorsToResume != 0) 
			{
				YieldProcessor();
			}   
		} 
		return 0;
	}
  
	//---------------------------------------------------------------------------------------------------------------
	VOID    InitSyncProcedure(
		CALLBACK TestDemo
	)
	{
		MCONTEXT* context			= (MCONTEXT*)ExAllocatePool(NonPagedPool, PAGE_SIZE);
		context->ProcessorId		= KeGetCurrentProcessorNumberEx(nullptr);
		context->RunningProcessor   = KeNumberProcessors - 1;
		context->ProcessorsToResume = KeNumberProcessors - 1;
		context->callback			= TestDemo;
		context->Params				= NULL;
		context->Done				= FALSE;

		//Send APIC IPI signal processors
		KeIpiGenericCall(BroadCaster, (ULONG_PTR)context);

		ExFreePool(context);
	}
}