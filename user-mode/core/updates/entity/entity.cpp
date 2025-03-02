#pragma once

#include <core/sdk/offsets.h>
#include <core/updates/entity.h>
#include <core/updates/world.h>
#include <core/updates/entity/entity.h>

void entity::run()
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    while (true)
    {
        world_data_ptr->gworld = km->read<uintptr_t>(km->module_base + offsets->GWorld);

        std::cout << "gworld -> " << world_data_ptr->gworld << std::endl;

        world_data_ptr->game_state = km->read<uintptr_t>(world_data_ptr->gworld + offsets->GameState);

        std::cout << "game_state -> " << world_data_ptr->game_state << std::endl;

        world_data_ptr->game_instance = km->read<uintptr_t>(world_data_ptr->gworld + offsets->GameInstance);

        std::cout << "game_instance -> " << world_data_ptr->game_instance << std::endl;

        world_data_ptr->local_player = km->read<uintptr_t>(km->read<uintptr_t>(world_data_ptr->game_instance + offsets->LocalPlayers));

        std::cout << "local_player -> " << world_data_ptr->local_player << std::endl;

        world_data_ptr->player_controller = km->read<uintptr_t>(world_data_ptr->local_player + offsets->PlayerController);

        std::cout << "player_controller -> " << world_data_ptr->player_controller << std::endl;

        world_data_ptr->local_pawn = km->read<uintptr_t>(world_data_ptr->player_controller + offsets->AcknowledgedPawn);

        std::cout << "local_pawn -> " << world_data_ptr->local_pawn << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}