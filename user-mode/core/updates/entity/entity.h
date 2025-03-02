#pragma once

#include <chrono>

class entity
{
public:

	void run();
}; inline std::unique_ptr<entity> u_entity = std::make_unique<entity>();
