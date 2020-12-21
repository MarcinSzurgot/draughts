#pragma once

#include <chrono>

#include "Game.hpp"
#include "Piece.hpp"
#include "Player.hpp"

struct RandomCpuPlayer : Player
{
    RandomCpuPlayer();

    void move(Game& game) override;

private:
    std::optional<Move> currentMove_;
    int step_;
    std::chrono::high_resolution_clock::time_point timeDelay_;
};