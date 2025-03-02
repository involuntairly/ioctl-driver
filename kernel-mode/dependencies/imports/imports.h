#pragma once

#include <dependencies/imports/ia32/ia32.h>

namespace imports
{
	struct driver_imports {
		uintptr_t
			rtl_init_unicode_string,
			zw_query_system_information,
			ex_allocate_pool,
			ps_lookup_process_by_proces_id,
			mm_copy_virtual_memory,
			mm_copy_memory,
			obf_dereference_object,
			io_allocate_mdl,
			mm_probe_and_lock_pages,
			mm_unlock_pages,
			io_free_mdl,
			mm_map_io_space_ex,
			mm_unmap_io_space,
			mm_get_physical_address,
			mm_map_locked_pages_specify_cache,
			mm_protect_mdl_system_address,
			mm_unmap_locked_pages,
			zw_open_key,
			zw_query_value_key,
			zw_close,
			ex_free_pool_with_tag,
			ps_lookup_process_by_process_id,
			io_get_current_process,
			mm_get_virtual_for_physical,
			mm_get_physical_memory_ranges,
			zw_allocate_virtual_memory,
			zw_protect_virtual_memory,
			zw_free_virtual_memory,
			zw_query_virtual_memory,
			rtl_avl_remove_node,
			ps_get_process_peb;
	};

	namespace globals
	{
		inline driver_imports imported{ };
	}

	NTSTATUS zw_query_system_information(
		SYSTEM_INFORMATION_CLASS SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength);

	PVOID ex_allocate_pool(
		POOL_TYPE PoolType,
		SIZE_T NumberOfBytes);

	NTSTATUS mm_copy_virtual_memory(
		PEPROCESS SourceProcess,
		PVOID SourceAddress,
		PEPROCESS TargetProcess,
		PVOID TargetAddress,
		SIZE_T BufferSize,
		KPROCESSOR_MODE PreviousMode
		, PSIZE_T ReturnSize);

	NTSTATUS mm_copy_memory(
		PVOID TargetAddress,
		MM_COPY_ADDRESS SourceAddress,
		SIZE_T NumberOfBytes,
		ULONG Flags,
		PSIZE_T NumberOfBytesTransferred);

	PVOID mm_map_io_space_ex(
		PHYSICAL_ADDRESS PhysicalAddress,
		SIZE_T NumberOfBytes,
		ULONG Protect);

	VOID rtl_init_unicode_string(
		PUNICODE_STRING DestinationString,
		PCWSTR SourceString);

	LONG_PTR obf_dereference_object(
		PVOID Object);

	PHYSICAL_ADDRESS mm_get_physical_address(
		PVOID BaseAddress);

	PMDL io_allocate_mdl(
		PVOID VirtualAddress,
		ULONG Length,
		BOOLEAN SecondaryBuffer,
		BOOLEAN ChargeQuota,
		PIRP Irp);

	PVOID mm_unmap_io_space(
		PVOID BaseAddress,
		SIZE_T NumberOfBytes);

	VOID mm_probe_and_lock_pages(
		PMDL MemoryDescriptorList,
		KPROCESSOR_MODE AccessMode,
		LOCK_OPERATION Operation);

	PVOID mm_map_locked_pages_specify_cache(
		PMDL MemoryDescriptorList,
		KPROCESSOR_MODE AccessMode,
		MEMORY_CACHING_TYPE CacheType,
		PVOID RequestedAddress,
		ULONG BugCheckOnFailure,
		ULONG Priority);

	NTSTATUS mm_protect_mdl_system_address(
		PMDL MemoryDescriptorList,
		ULONG NewProtect);

	VOID mm_unmap_locked_pages(
		PVOID BaseAddress,
		PMDL MemoryDescriptorList);

	VOID mm_unlock_pages(
		PMDL MemoryDescriptorList);

	VOID io_free_mdl(
		PMDL Mdl);

	NTSTATUS zw_open_key(
		PHANDLE KeyHandle,
		ACCESS_MASK DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes);

	NTSTATUS zw_query_value_key(
		HANDLE KeyHandle,
		PUNICODE_STRING ValueName,
		KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
		PVOID KeyValueInformation,
		ULONG Length,
		PULONG ResultLength);

	NTSTATUS zw_close(
		HANDLE Handle);

	VOID ex_free_pool_with_tag(
		PVOID P,
		ULONG Tag);

	NTSTATUS ps_lookup_process_by_process_id(
		HANDLE ProcessId,
		PEPROCESS* Process);

	PEPROCESS io_get_current_process();

	PVOID mm_get_virtual_for_physical(
		PHYSICAL_ADDRESS PhysicalAddress);

	PPHYSICAL_MEMORY_RANGE mm_get_physical_memory_ranges();

	NTSTATUS zw_allocate_virtual_memory(
		HANDLE ProcessHandle,
		PVOID* BaseAddress,
		ULONG_PTR ZeroBits,
		PSIZE_T RegionSize,
		ULONG AllocationType,
		ULONG Protect);

	NTSTATUS zw_protect_virtual_memory(
		HANDLE ProcessHandle,
		PVOID* BaseAddress,
		PULONG NumberOfBytesToProtect,
		ULONG NewAccessProtection,
		PULONG OldAccessProtection);

	NTSTATUS zw_free_virtual_memory(HANDLE ProcessHandle,
		PVOID* BaseAddress,
		PSIZE_T RegionSize,
		ULONG FreeType);

	NTSTATUS zw_query_virtual_memory(
		HANDLE ProcessHandle,
		PVOID BaseAddress,
		MEMORY_INFORMATION_CLASS MemoryInformationClass,
		PVOID MemoryInformation,
		SIZE_T MemoryInformationLength,
		PSIZE_T ReturnLength);

	PVOID rtl_avl_remove_node(PRTL_AVL_TREE pTree,
		PMMADDRESS_NODE pNode);

	PPEB ps_get_process_peb(
		PEPROCESS Process);
}