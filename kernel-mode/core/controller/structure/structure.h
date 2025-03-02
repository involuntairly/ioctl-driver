#pragma once 

#include <cstdint>
#include <ntifs.h>

namespace structures
{
	typedef struct module_base
	{
		int32_t process_id;
		uint64_t* address;
	}mb, * mba;

	typedef struct read_write
	{
		int32_t process_id;
		uint64_t address;
		uint64_t buffer;
		uint64_t size;
		bool write;
	}rw, * rwa;

	typedef struct page_table
	{
		int32_t process_id;
		uint64_t address;
	}pt, * pta;
}