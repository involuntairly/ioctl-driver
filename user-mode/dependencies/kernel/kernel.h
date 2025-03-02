#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

#include <dependencies/kernel/structure/structure.h>

class km_f {
public:

	HANDLE device_handle;

	INT32 process_id;

	uintptr_t module_base;

	bool setup_driver();

	uintptr_t attach_process(
		LPCTSTR process_name);

	uintptr_t get_base_address();

	uintptr_t get_dtb();

	bool read_physical_address(
		void* address, void* buffer, 
		DWORD size);

	bool write_physical_address(
		void* address, 
		void* buffer, 
		DWORD size);

	template <typename T>
	T read(uint64_t address)
	{
		T buffer{ };
		read_physical_address(
			(void*)address, 
			&buffer, 
			sizeof(T));
		return buffer;
	}

	template <typename T>
	bool write(uint64_t address, const T& data)
	{
		return write_physical_address(
			(void*)address, 
			(void*)&data, 
			sizeof(T));
	}

	void read_array(
		uintptr_t address, 
		void* buffer, 
		std::size_t size)
	{
		if (size > 0)
		{
			read_physical_address(
				(void*)address, 
				buffer, 
				size);
		}
	}

	[[nodiscard]] auto read_string(
		const std::uintptr_t addr,
		void* buffer,
		std::size_t size) -> std::string {

		this->read_array(addr, buffer, size);
		char name[255] = { 0 };
		std::memcpy(&name, buffer, size);
		return std::string(name);
	}
};

inline std::unique_ptr<km_f> km = std::make_unique<km_f>();