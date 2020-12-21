#pragma once

#include "Game.hpp"
#include "Piece.hpp"

struct Player
{
    virtual void move(Game& game) = 0;
};