#pragma once

#include <chrono>
#include <string>
#include <unordered_set>

struct entity_data {
    uintptr_t player_state = 0;
    uintptr_t pawn_private = 0;
};