#pragma once
#include <iostream>
#include <thread>

#include <dependencies/cipher/hash.h>
#include <dependencies/kernel/kernel.h>

#include <core/updates/entity/entity.h>

int entry_point()
{
	std::cout << hash_str("initalizing driver communication...\n") << std::endl;

	Sleep(500);

	km->setup_driver();

	km->process_id = km->attach_process(hash_str(L"FortniteClient-Win64-Shipping.exe"));

	km->module_base = km->get_base_address();

	printf(hash_str("\nprocess_id: %d\n"), km->process_id);

	printf(hash_str("\nbase_address: %p\n"), km->module_base);

	km->get_dtb();

	entity entity_instance;

	std::thread(&entity::run, &entity_instance).detach();

	// we're creating a thread that'll never quite so the code below is redundant and mainly for a if else sitation

	std::cin.get();

	return 0;
}