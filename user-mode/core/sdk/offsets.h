#pragma once

#include <memory>

class offset {
public:
    static constexpr uintptr_t GWorld = 0x169641b8;

    static constexpr uintptr_t GameInstance = 0x218;

    static constexpr uintptr_t LocalPlayers = 0x38;

    static constexpr uintptr_t PlayerController = 0x30;

    static constexpr uintptr_t AcknowledgedPawn = 0x350;

    static constexpr uintptr_t PlayerState = 0x2c8;

    static constexpr uintptr_t GameState = 0x1a0;
};

inline std::unique_ptr<offset> offsets = std::make_unique<offset>();