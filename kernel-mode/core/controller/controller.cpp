#pragma once

#include <core/controller/controller.h>
#include <core/features/module/module.h>
#include <core/features/memory/memory.h>
#include <core/features/paging/paging.h>

#include <intrin.h>
#include <strsafe.h> 

NTSTATUS controller::ioctl_controller(PDEVICE_OBJECT device_object, PIRP irp) {
	UNREFERENCED_PARAMETER(device_object);
	PIO_STACK_LOCATION invoke = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status = { };
	ULONG bytes = { };

	ULONG code = invoke->Parameters.DeviceIoControl.IoControlCode;
	ULONG size = invoke->Parameters.DeviceIoControl.InputBufferLength;

	if (code == code_rw)
	{
		if (size == sizeof(structures::rw))
		{
			structures::rwa req = (structures::rwa)(irp->AssociatedIrp.SystemBuffer);
			if (req->write) {
				if (memory::write_physical_memory(req->process_id, req->address, (void*)req->buffer, req->size)) {
					return STATUS_SUCCESS;
				}
			}
			else {
				if (memory::read_physical_memory(req->process_id, req->address, (void*)req->buffer, req->size)) {
					return STATUS_SUCCESS;
				}
			}
			bytes = sizeof(structures::rw);
		}
	}

	else if (code == code_mb)
	{
		if (size == sizeof(structures::mb))
		{
			structures::mba req = (structures::mba)(irp->AssociatedIrp.SystemBuffer);
			status = module::get_module(req);
			bytes = sizeof(structures::mb);
		}
	}
	else if (code == code_pt)
	{
		structures::pta req = (structures::pta)(irp->AssociatedIrp.SystemBuffer);

		status = STATUS_SUCCESS;
		paging::update::update_cache((HANDLE)req->process_id);
		bytes = sizeof(structures::pta);
	}
	else {
		status = STATUS_INFO_LENGTH_MISMATCH;
		bytes = 0;
	}

	irp->IoStatus.Status = status;
	irp->IoStatus.Information = bytes;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS controller::handle_dispatch(PDEVICE_OBJECT device_object, PIRP irp) {
	UNREFERENCED_PARAMETER(device_object);

	PIO_STACK_LOCATION invoke = IoGetCurrentIrpStackLocation(irp);

	switch (invoke->MajorFunction)
	{
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE:
		break;
	default:
		break;
	}

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS controller::not_supported(PDEVICE_OBJECT device_object, PIRP irp) {
	UNREFERENCED_PARAMETER(device_object);
	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

UNICODE_STRING controller::concatenate_strings(const wchar_t* str1, const wchar_t* str2) {
	UNICODE_STRING result;
	RtlInitUnicodeString(&result, nullptr);

	size_t length1 = wcslen(str1);
	size_t length2 = wcslen(str2);
	size_t totalLength = length1 + length2;

	result.Buffer = (wchar_t*)ExAllocatePool(NonPagedPool, (totalLength + 1) * sizeof(wchar_t));

	if (result.Buffer)
	{
		result.Length = (USHORT)(totalLength * sizeof(wchar_t));
		result.MaximumLength = (USHORT)((totalLength + 1) * sizeof(wchar_t));

		memcpy(result.Buffer, str1, length1 * sizeof(wchar_t));

		memcpy(result.Buffer + length1, str2, (length2 + 1) * sizeof(wchar_t));
	}

	return result;
}

UNICODE_STRING u_name, u_link;

wchar_t								w_device_name[260];
wchar_t								W_link_name[260];
WCHAR				w_rand_str[128];

NTSTATUS controller::initiate_driver(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
	UNREFERENCED_PARAMETER(registry_path);

	NTSTATUS status = { };
	PDEVICE_OBJECT device_obj = { };

	int	id[4];
	__cpuid(id, 1);
	wchar_t w_buffer[1];

	StringCchPrintfW(w_rand_str, sizeof(w_rand_str), /*encrypt*/(L"%08X"), id[0]);

	StringCchPrintfW(w_device_name, sizeof(w_device_name), /*encrypt*/(L"\\Device\\%s"), w_rand_str);
	StringCchPrintfW(W_link_name, sizeof(W_link_name), /*encrypt*/(L"\\DosDevices\\%s"), w_rand_str);

	RtlInitUnicodeString(&u_name, w_device_name);
	RtlInitUnicodeString(&u_link, W_link_name);

	status = IoCreateDevice(driver_object, 0, &u_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_obj);

	if (!NT_SUCCESS(status))
		return status;

	status = IoCreateSymbolicLink(&u_link, &u_name);

	if (!NT_SUCCESS(status))
		return status;

	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
		driver_object->MajorFunction[i] = &controller::not_supported;

	device_obj->Flags |= DO_BUFFERED_IO;

	driver_object->MajorFunction[IRP_MJ_CREATE] = &controller::handle_dispatch;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = &controller::handle_dispatch;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &controller::ioctl_controller;

	device_obj->Flags &= ~DO_DEVICE_INITIALIZING;

	return status;
}