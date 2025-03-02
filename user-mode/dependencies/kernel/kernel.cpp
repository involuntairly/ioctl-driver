#pragma once

#include <unordered_map>

#include <tchar.h>

#include <dependencies/kernel/kernel.h>

#include <dependencies/cipher/hash.h>

bool km_f::setup_driver() {
	int	id[4];
	__cpuid(id, 1);
	wchar_t buffer[1];

	wsprintfW(buffer, hash_str(L"\\\\.\\\%08X"), id[0]);

	km->device_handle = CreateFileW(
		buffer, GENERIC_READ
		| GENERIC_WRITE, 
		FILE_SHARE_READ | 
		FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING,
		0, 
		NULL);

	if (!km->device_handle || km->device_handle == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

uintptr_t km_f::attach_process(LPCTSTR process_name) {
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hsnap, &pt)) {
		do {
			if (_tcsicmp(pt.szExeFile, process_name) == 0) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}

	CloseHandle(hsnap);

	return NULL;
}

uintptr_t km_f::get_base_address()
{
	uintptr_t image_address = { NULL };
	structures::mb arguments = { NULL };
	arguments.process_id = process_id;
	arguments.address = (ULONGLONG*)&image_address;

	DeviceIoControl(device_handle, code_mb, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	return image_address;
}

uintptr_t km_f::get_dtb()
{
	structures::pt arguments = { NULL };
	arguments.process_id = process_id;

	return DeviceIoControl(device_handle, code_pt, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

bool km_f::read_physical_address(PVOID address, PVOID buffer, DWORD size)
{
	structures::rw arguments = { 0 };
	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = process_id;

	return DeviceIoControl(device_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

bool km_f::write_physical_address(PVOID address, PVOID buffer, DWORD size)
{

	structures::rw arguments = { 0 };

	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = process_id;
	arguments.write = true;

	return DeviceIoControl(device_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}