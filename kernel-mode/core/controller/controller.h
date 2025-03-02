#pragma once

#include <dependencies/includes.h>

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9001, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define code_mb CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9002, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define code_pt CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9003, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

namespace controller {
	NTSTATUS ioctl_controller
	(PDEVICE_OBJECT device_object, 
		PIRP irp);

	NTSTATUS handle_dispatch(
		PDEVICE_OBJECT device_object, 
		PIRP irp);

	NTSTATUS not_supported(
		PDEVICE_OBJECT device_object, 
		PIRP irp);

	UNICODE_STRING concatenate_strings(
		const wchar_t* string_1, 
		const wchar_t* string_2);

	NTSTATUS initiate_driver(
		PDRIVER_OBJECT driver_object, 
		PUNICODE_STRING registry_path);
}