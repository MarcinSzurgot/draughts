#pragma once

#include <chrono>

#include "Game.hpp"

struct RandomCpuPlayer
{
    RandomCpuPlayer();

    void update(Game& game);

private:
    std::optional<Move> currentMove_;
    int step_;
    std::chrono::high_resolution_clock::time_point timeDelay_;
};