#pragma once

#include <ntifs.h>
#include <windef.h>
#include <cstdint>

#include <dependencies/skcrypt/skcrypter.h>

#include <core/controller/structure/structure.h>

// yes we're using NTAPI, yes, you can do PEProcess + 0x520 (SectionBaseAddress)

extern "C" PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);

// and yes, you can also fastcall this to avoid logging with (PsLoadedModuleList) and bypass (EtwTiLogDriverObjectLoad)
// with https://github.com/Th3Spl/IoCreateDriver (after a little bit of tweaking), since it is a POC and natively has issues. 

extern "C" NTSTATUS NTAPI IoCreateDriver(PUNICODE_STRING driver_name, PDRIVER_INITIALIZE init_func);

namespace data
{
    extern UINT64 cached_pml4[512];
    extern uint64_t saved_cr3;
}