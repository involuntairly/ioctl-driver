#pragma once

#include <core/controller/controller.h>

extern "C" NTSTATUS entry_point(PDRIVER_OBJECT driver_object, PUNICODE_STRING  registry_path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	// since we're manual mapping these are both invalid *-w-*

	// credits -> sqqrky on discord/github, if you plan on using this i recommend importing system calls, and not using a driver/device object
	// this was just a scrap project, made this in less than 2 hours, the driver should be fine but the usermode fails to build on my end, i probably corrupted something, fix it. 

	return IoCreateDriver(nullptr, reinterpret_cast<PDRIVER_INITIALIZE>(controller::initiate_driver));

	return true;
}