#pragma once

#include <dependencies/includes.h>

namespace memory {
	bool read_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		SIZE_T* bytes);

	bool write_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		SIZE_T* bytes);

	std::uint64_t translate_linear_address(
		std::uint64_t cr3,
		std::uint64_t virtual_address);

	bool read_physical_memory(
		std::uint64_t cr3,
		std::uint64_t address,
		void* buffer,
		std::uint64_t size);

	bool write_physical_memory(
		std::uint64_t cr3,
		std::uint64_t address,
		void* buffer,
		std::uint64_t size);
}