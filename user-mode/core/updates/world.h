#pragma once
#include <cstdint>
#include <windows.h>
#include <vector>
#include <thread>
#include <string>

#include <dependencies/kernel/kernel.h>

struct world_data {
    uintptr_t gworld = 0;
    uintptr_t game_instance = 0;
    uintptr_t game_state = 0;
    uintptr_t local_player = 0;
    uintptr_t local_pawn = 0;
    uintptr_t player_controller = 0;
    uintptr_t player_array = 0;
    int player_array_size = 0;
};

inline auto world_data_ptr = std::make_unique<world_data>();