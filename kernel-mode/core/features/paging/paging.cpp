#pragma once

#include <core/features/paging/paging.h>

#include <dependencies/imports/imports.h>

bool paging::read_physical_address(std::uint64_t address, void* buffer, std::uint64_t size, SIZE_T* bytes) {
    MM_COPY_ADDRESS address_to_read = { };
    address_to_read.PhysicalAddress.QuadPart = address;

    return NT_SUCCESS(MmCopyMemory(buffer, address_to_read, size, (MM_COPY_MEMORY_PHYSICAL), bytes));
}

void* paging::search_memory(void* search_base, SIZE_T search_size, const void* pattern, SIZE_T pattern_size)
{
    const UCHAR* search_base_var = static_cast<const UCHAR*>(search_base);
    const UCHAR* pattern_var = static_cast<const UCHAR*>(pattern);

    for (SIZE_T i = 0; i <= search_size - pattern_size; ++i)
    {
        SIZE_T j = 0;
        for (; j < pattern_size; ++j)
        {
            if (search_base_var[i + j] != pattern_var[j])
                break;
        }
        if (j == pattern_size)
        {
            return const_cast<UCHAR*>(&search_base_var[i]);
        }
    }
    return nullptr;
}

bool paging::initialize_mmpfn_database() {
    struct mm_pfn_database_search_pattern
    {
        const UCHAR* bytes;
        SIZE_T bytes_size;
        bool hard_coded;
    };

    static const UCHAR pattern_bytes[] = {
        0x48, 0x8B, 0xC1,        // mov     rax, rcx
        0x48, 0xC1, 0xE8, 0x0C, // shr     rax, 0Ch
        0x48, 0x8D, 0x14, 0x40, // lea     rdx, [rax + rax * 2]
        0x48, 0x03, 0xD2,        // add     rdx, rdx
        0x48, 0xB8,                // mov     rax, 0FFFFFA8000000008h
    };

    mm_pfn_database_search_pattern patterns
    {
        pattern_bytes,
        sizeof(pattern_bytes),
        true
    };

    const auto mm_get_virtual_for_physical = reinterpret_cast<UCHAR*>(MmGetVirtualForPhysical);
    if (!mm_get_virtual_for_physical) {
        return false;
    }

    auto found = reinterpret_cast<UCHAR*>(search_memory(mm_get_virtual_for_physical, 0x20, patterns.bytes, patterns.bytes_size));
    if (!found) {
        return false;
    }

    found += patterns.bytes_size;
    if (patterns.hard_coded)
    {
        mm_pfn_database = *reinterpret_cast<void**>(found);
    }
    else
    {
        const auto mmpfn_address = *reinterpret_cast<ULONG_PTR*>(found);
        mm_pfn_database = *reinterpret_cast<void**>(mmpfn_address);
    }

    mm_pfn_database = PAGE_ALIGN(mm_pfn_database);
    return true;
}

uintptr_t paging::dirbase_from_handle(void* module_handle) {
    if (!NT_SUCCESS(initialize_mmpfn_database()))
        return 0;

    VIRTUAL_ADDRESS virt_base{ };
    virt_base.value = module_handle;

    size_t read{};
    PPHYSICAL_MEMORY_RANGE physical_ranges = MmGetPhysicalMemoryRanges();
    for (int i = 0;; i++) {
        auto physical_range = &physical_ranges[i];
        if (!physical_range->BaseAddress.QuadPart || !physical_range->NumberOfBytes.QuadPart)
            break;

        uintptr_t current_phys_address = physical_range->BaseAddress.QuadPart;
        for (int j = 0; j < (physical_range->NumberOfBytes.QuadPart / 0x1000); j++, current_phys_address += 0x1000) {
            auto pnf_info = (_MMPFN*)((uintptr_t)mm_pfn_database + (current_phys_address >> 12) * sizeof(_MMPFN));
            if (pnf_info->u4.PteFrame == (current_phys_address >> 12)) {
                MMPTE pml4e{};
                if (!NT_SUCCESS(read_physical_address(
                    current_phys_address + 8 * virt_base.pml4_index,
                    &pml4e, 8, &read))) {
                    continue;
                }

                if (!pml4e.u.Hard.Valid)
                    continue;

                MMPTE pdpte{};
                if (!NT_SUCCESS(read_physical_address(
                    (pml4e.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pdpt_index,
                    &pdpte, 8, &read))) {
                    continue;
                }

                if (!pdpte.u.Hard.Valid) {
                    continue;
                }

                MMPTE pde{};
                if (!NT_SUCCESS(read_physical_address(
                    (pdpte.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pd_index,
                    &pde, 8, &read))) {
                    continue;
                }

                if (!pde.u.Hard.Valid) {
                    continue;
                }

                MMPTE pte{};
                if (!NT_SUCCESS(read_physical_address(
                    (pde.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pt_index,
                    &pte, 8, &read))) {
                    continue;
                }

                if (!pte.u.Hard.Valid) {
                    continue;
                }

                return current_phys_address;
            }
        }
    }

    return 0;
}

void paging::update::update_cache(HANDLE process_id) {
    size_t bytes{};
    PEPROCESS process{};
    PsLookupProcessByProcessId(process_id, &process);
    data::saved_cr3 = dirbase_from_handle(PsGetProcessSectionBaseAddress(process));

    for (int i = 0; i < 512; i++)
    {
        read_physical_address(data::saved_cr3 + 8 * i, &data::cached_pml4[i], 8, &bytes);
    }
    ObDereferenceObject(process);
}