#pragma once

#include <dependencies/includes.h>

namespace paging {
	bool read_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		SIZE_T* bytes);

	void* search_memory(
		void* start,
		SIZE_T size,
		const void* pattern,
		SIZE_T pattern_size);

	bool initialize_mmpfn_database(
	);

	uintptr_t dirbase_from_handle(
		void* module_handle);

	namespace update {
		void update_cache(
			HANDLE process_id);
	}

	inline void* mm_pfn_database;
}