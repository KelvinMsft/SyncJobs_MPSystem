#pragma once

/// Sets a break point that works only when a debugger is present
#if !defined(APIC_IPI_COMMON_DBG_BREAK)
#define APIC_IPI_COMMON_DBG_BREAK() \
  if (KD_DEBUGGER_NOT_PRESENT) {         \
  } else {                               \
    __debugbreak();                      \
  }                                      \
  reinterpret_cast<void*>(0)
#endif

#define APIC_IPI_DEBUG_INFO(format, ...) DbgPrintEx(0,0,format,__VA_ARGS__)
