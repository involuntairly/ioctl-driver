#pragma once

#include <core/features/module/module.h>

NTSTATUS module::get_module(structures::mba module) {
    if (!module->process_id)
        return STATUS_UNSUCCESSFUL;

    PEPROCESS process;
    if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)module->process_id, &process)))
        return STATUS_UNSUCCESSFUL;

    ULONGLONG image_base = (ULONGLONG)PsGetProcessSectionBaseAddress(process);

    RtlCopyMemory(module->address, &image_base, sizeof(image_base));
    ObDereferenceObject(process);

    return STATUS_SUCCESS;
}