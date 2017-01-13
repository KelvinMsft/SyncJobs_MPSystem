#pragma once
#include <fltKernel.h>

typedef ULONG (_IRQL_requires_min_(IPI_LEVEL) __stdcall *CALLBACK)(PVOID);
 
#pragma pack(8)
typedef struct MCONTEXT
{
	ULONG		   ProcessorId;
	volatile LONG  RunningProcessor;
	volatile LONG  ProcessorsToResume;
	ULONG		   Done; 
	CALLBACK	   callback;
	PVOID		   Params;
}MCONTEXT, *PMCONTEXT;
#pragma pack()

VOID    InitSyncProcedure(CALLBACK callback);
