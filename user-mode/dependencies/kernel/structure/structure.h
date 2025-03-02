#pragma once 

#include <cstdint>
#include <Intrin.h>

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9001, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define code_mb CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9002, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define code_pt CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9003, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

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