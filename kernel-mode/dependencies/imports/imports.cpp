#pragma once

#include <dependencies/imports/imports.h>

namespace imports
{
	NTSTATUS zw_query_system_information(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
	{
		return reinterpret_cast<NTSTATUS(*)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG)> (globals::imported.zw_query_system_information)(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}

	PVOID ex_allocate_pool(POOL_TYPE PoolType, SIZE_T NumberOfBytes)
	{
		return reinterpret_cast<PVOID(*)(POOL_TYPE, SIZE_T)>(globals::imported.ex_allocate_pool)(PoolType, NumberOfBytes);
	}

	NTSTATUS mm_copy_virtual_memory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize)
	{
		return reinterpret_cast<NTSTATUS(*)(PEPROCESS, PVOID, PEPROCESS, PVOID, SIZE_T, KPROCESSOR_MODE, PSIZE_T)> (globals::imported.mm_copy_virtual_memory)(SourceProcess, SourceAddress, TargetProcess, TargetAddress, BufferSize, PreviousMode, ReturnSize);
	}

	NTSTATUS mm_copy_memory(PVOID TargetAddress, MM_COPY_ADDRESS SourceAddress, SIZE_T NumberOfBytes, ULONG Flags, PSIZE_T NumberOfBytesTransferred)
	{
		return reinterpret_cast<NTSTATUS(*)(PVOID, MM_COPY_ADDRESS, SIZE_T, ULONG, PSIZE_T)> (globals::imported.mm_copy_memory)(TargetAddress, SourceAddress, NumberOfBytes, Flags, NumberOfBytesTransferred);
	}

	PVOID mm_map_io_space_ex(PHYSICAL_ADDRESS PhysicalAddress, SIZE_T NumberOfBytes, ULONG Protect)
	{
		return reinterpret_cast<PVOID(*)(PHYSICAL_ADDRESS, SIZE_T, ULONG)> (globals::imported.mm_map_io_space_ex)(PhysicalAddress, NumberOfBytes, Protect);
	}

	PVOID mm_unmap_io_space(PVOID BaseAddress, SIZE_T NumberOfBytes)
	{
		return reinterpret_cast<PVOID(*)(PVOID, SIZE_T)> (globals::imported.mm_unmap_io_space)(BaseAddress, NumberOfBytes);
	}

	VOID rtl_init_unicode_string(PUNICODE_STRING DestinationString, PCWSTR SourceString)
	{
		return reinterpret_cast<VOID(*)(PUNICODE_STRING, PCWSTR)>(globals::imported.rtl_init_unicode_string)(DestinationString, SourceString);
	}

	LONG_PTR obf_dereference_object(PVOID Object)
	{
		return reinterpret_cast<LONG_PTR(*)(PVOID)>(globals::imported.obf_dereference_object)(Object);
	}

	PHYSICAL_ADDRESS mm_get_physical_address(PVOID BaseAddress)
	{
		return reinterpret_cast<PHYSICAL_ADDRESS(*)(PVOID)> (globals::imported.mm_get_physical_address)(BaseAddress);
	}

	PMDL io_allocate_mdl(PVOID VirtualAddress, ULONG Length, BOOLEAN SecondaryBuffer, BOOLEAN ChargeQuota, PIRP Irp)
	{
		return reinterpret_cast<PMDL(*)(PVOID, ULONG, BOOLEAN, BOOLEAN, PIRP)>(globals::imported.io_allocate_mdl)(VirtualAddress, Length, SecondaryBuffer, ChargeQuota, Irp);
	}

	VOID mm_probe_and_lock_pages(PMDL MemoryDescriptorList, KPROCESSOR_MODE AccessMode, LOCK_OPERATION Operation)
	{
		return reinterpret_cast<VOID(*)(PMDL, KPROCESSOR_MODE, LOCK_OPERATION)>(globals::imported.mm_probe_and_lock_pages)(MemoryDescriptorList, AccessMode, Operation);
	}

	PVOID mm_map_locked_pages_specify_cache(PMDL MemoryDescriptorList, KPROCESSOR_MODE AccessMode, MEMORY_CACHING_TYPE CacheType, PVOID RequestedAddress, ULONG BugCheckOnFailure, ULONG Priority)
	{
		return reinterpret_cast<PVOID(*)(PMDL, KPROCESSOR_MODE, MEMORY_CACHING_TYPE, PVOID, ULONG, ULONG)>(globals::imported.mm_map_locked_pages_specify_cache)(MemoryDescriptorList, AccessMode, CacheType, RequestedAddress, BugCheckOnFailure, Priority);
	}

	NTSTATUS mm_protect_mdl_system_address(PMDL MemoryDescriptorList, ULONG NewProtect)
	{
		return reinterpret_cast<NTSTATUS(*)(PMDL, ULONG)>(globals::imported.mm_protect_mdl_system_address)(MemoryDescriptorList, NewProtect);
	}

	VOID mm_unmap_locked_pages(PVOID BaseAddress, PMDL MemoryDescriptorList)
	{
		return reinterpret_cast<VOID(*)(PVOID, PMDL)>(globals::imported.mm_unmap_locked_pages)(BaseAddress, MemoryDescriptorList);
	}

	VOID mm_unlock_pages(PMDL MemoryDescriptorList)
	{
		return reinterpret_cast<VOID(*)(PMDL)>(globals::imported.mm_unlock_pages)(MemoryDescriptorList);
	}

	VOID io_free_mdl(PMDL Mdl)
	{
		return reinterpret_cast<VOID(*)(PMDL)>(globals::imported.io_free_mdl)(Mdl);
	}

	NTSTATUS zw_open_key(PHANDLE KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes)
	{
		return reinterpret_cast<NTSTATUS(*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES)> (globals::imported.zw_open_key)(KeyHandle, DesiredAccess, ObjectAttributes);
	}

	NTSTATUS zw_query_value_key(HANDLE KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ResultLength)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PUNICODE_STRING, KEY_VALUE_INFORMATION_CLASS, PVOID, ULONG, PULONG)> (globals::imported.zw_query_value_key)(KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, Length, ResultLength);
	}

	NTSTATUS zw_close(HANDLE Handle)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE)> (globals::imported.zw_close)(Handle);
	}

	VOID ex_free_pool_with_tag(PVOID P, ULONG Tag)
	{
		reinterpret_cast<VOID(*)(PVOID, ULONG)> (globals::imported.ex_free_pool_with_tag)(P, Tag);
	}

	NTSTATUS ps_lookup_process_by_process_id(HANDLE ProcessId, PEPROCESS* Process)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PEPROCESS*)> (globals::imported.ps_lookup_process_by_process_id)(ProcessId, Process);
	}

	PEPROCESS io_get_current_process()
	{
		return reinterpret_cast<PEPROCESS(*)()> (globals::imported.io_get_current_process)();
	}

	PVOID mm_get_virtual_for_physical(PHYSICAL_ADDRESS PhysicalAddress)
	{
		return reinterpret_cast<PVOID(*)(PHYSICAL_ADDRESS)> (globals::imported.mm_get_virtual_for_physical)(PhysicalAddress);
	}

	PPHYSICAL_MEMORY_RANGE mm_get_physical_memory_ranges()
	{
		return reinterpret_cast<PPHYSICAL_MEMORY_RANGE(*)()> (globals::imported.mm_get_physical_memory_ranges)();
	}

	NTSTATUS zw_allocate_virtual_memory(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG)> (globals::imported.zw_allocate_virtual_memory)(ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
	}

	NTSTATUS zw_protect_virtual_memory(HANDLE ProcessHandle, PVOID* BaseAddress, PULONG NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PVOID*, PULONG, ULONG, PULONG)> (globals::imported.zw_protect_virtual_memory)(ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
	}

	NTSTATUS zw_free_virtual_memory(HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T RegionSize, ULONG FreeType)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PVOID*, PSIZE_T, ULONG)> (globals::imported.zw_free_virtual_memory)(ProcessHandle, BaseAddress, RegionSize, FreeType);
	}

	NTSTATUS zw_query_virtual_memory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength)
	{
		return reinterpret_cast<NTSTATUS(*)(HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T)> (globals::imported.zw_query_virtual_memory)(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
	}

	PVOID rtl_avl_remove_node(PRTL_AVL_TREE pTree, PMMADDRESS_NODE pNode)
	{
		return reinterpret_cast<PVOID(*)(PRTL_AVL_TREE, PMMADDRESS_NODE)> (globals::imported.rtl_avl_remove_node)(pTree, pNode);
	}

	PPEB ps_get_process_peb(PEPROCESS Process)
	{
		return reinterpret_cast<PPEB(*)(PEPROCESS)> (globals::imported.ps_get_process_peb)(Process);
	}
}