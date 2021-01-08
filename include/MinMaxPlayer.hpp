#pragma once

#include "Player.hpp"

#include <chrono>

struct MinMaxPlayer : Player
{
    MinMaxPlayer(int maxDepth);

    void move(Game& game) override;

private:
    int maxDepth_;

    std::optional<Move> currentMove_;
    int step_;
    std::chrono::high_resolution_clock::time_point timeDelay_;
};