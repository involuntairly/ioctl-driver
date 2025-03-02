#pragma once

#include <core/features/memory/memory.h>

#include <dependencies/imports/imports.h>

#include <dependencies/crt/crt.h>

// if you're curious why this was redefined, inside "dependencies/includes.h" it's an extern

namespace data
{
    uint64_t cached_pml4[512] = { 0 };
    uint64_t saved_cr3 = 0;
}

bool memory::read_physical_address(std::uint64_t address, void* buffer, std::uint64_t size, SIZE_T* bytes) {
    MM_COPY_ADDRESS address_to_read = { };
    address_to_read.PhysicalAddress.QuadPart = address;

    return NT_SUCCESS(MmCopyMemory(buffer, address_to_read, size, (MM_COPY_MEMORY_PHYSICAL), bytes));
}

bool memory::write_physical_address(std::uint64_t address, void* buffer, std::uint64_t size, SIZE_T* bytes) {
    PHYSICAL_ADDRESS address_to_write = { };
    address_to_write.QuadPart = address;

    void* mapped_page = MmMapIoSpaceEx(address_to_write, size, (PAGE_READWRITE));
    if (!mapped_page) {
        return false;
    }

    *bytes = size;
    memcpy(mapped_page, buffer, size);

    MmUnmapIoSpace(mapped_page, size);
    return true;
}

std::uint64_t memory::translate_linear_address(std::uint64_t cr3, std::uint64_t virtual_address) {
    cr3 &= (~0xf);

    std::uint64_t page_offset = virtual_address & (~(~0ul << 12));
    std::uint64_t pte = (virtual_address >> (12)) & (0x1ffll);
    std::uint64_t pt = (virtual_address >> (21)) & (0x1ffll);
    std::uint64_t pd = (virtual_address >> (30)) & (0x1ffll);
    std::uint64_t pdp = (virtual_address >> (39)) & (0x1ffll);

    SIZE_T read_size = 0;
    UINT64 pdpe = data::cached_pml4[pdp];
    read_physical_address(cr3 + (8) * pdp, &pdpe, sizeof(pdpe), &read_size);
    if (~pdpe & 1) {
        return 0;
    }

    std::uint64_t pde = 0;
    read_physical_address((pdpe & (~0xfull << 8) & (0xfffffffffull)) + (8) * pd, &pde, sizeof(pde), &read_size);
    if (~pde & 1) {
        return 0;
    }

    if (pde & (0x80)) {
        return (pde & (~0ull << 42 >> 12)) + (virtual_address & (~(~0ull << 30)));
    }

    std::uint64_t pte_address = 0;
    read_physical_address((pde & (~0xfull << 8) & 0xfffffffffull) + (8) * pt, &pte_address, sizeof(pte_address), &read_size);
    if (~pte_address & (1)) {
        return 0;
    }

    if (pte_address & (0x80)) {
        return (pte_address & (~0xfull << 8) & 0xfffffffffull) + (virtual_address & (~(~0ull << 21)));
    }

    virtual_address = 0;
    read_physical_address((pte_address & (~0xfull << 8) & 0xfffffffffull) + (8) * pte, &virtual_address, sizeof(virtual_address), &read_size);
    if (!virtual_address) {
        return 0;
    }

    virtual_address &= ((~0xfull << 8) & 0xfffffffffull);
    return virtual_address + page_offset;
}

bool memory::read_physical_memory(std::uint64_t cr3, std::uint64_t address, void* buffer, std::uint64_t size) {
    std::uint64_t current_offset = 0;
    std::uint64_t total_size = size;

    while (total_size) {
        std::uint64_t current_physical_address = translate_linear_address(cr3, address + current_offset);
        if (!current_physical_address) {
            return false;
        }

        std::uint64_t bytes_read = 0;
        std::uint64_t read_size = min((PAGE_SIZE)-(current_physical_address & (0xFFF)), total_size);
        if (!read_physical_address(current_physical_address, (void*)(std::uint64_t(buffer) + current_offset),
            read_size, &bytes_read) || !bytes_read)
        {
            return false;
        }

        total_size -= bytes_read, current_offset += bytes_read;
    }

    return true;
}

bool memory::write_physical_memory(std::uint64_t cr3, std::uint64_t address, void* buffer, std::uint64_t size) {
    std::uint64_t current_offset = 0;
    std::uint64_t total_size = size;

    while (total_size) {
        std::uint64_t current_physical_address = translate_linear_address(cr3, address + current_offset);
        if (!current_physical_address) {
            return false;
        }

        std::uint64_t bytes_read = 0;
        std::uint64_t read_size = min((PAGE_SIZE)-(current_physical_address & (0xFFF)), total_size);
        if (!write_physical_address(current_physical_address, (void*)(std::uint64_t(buffer) + current_offset),
            read_size, &bytes_read) || !bytes_read)
        {
            return false;
        }

        total_size -= bytes_read, current_offset += bytes_read;
    }
}