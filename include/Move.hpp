#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Piece.hpp"

struct GameState;
struct Move;

struct Move
{
    sf::Vector2i from;
    sf::Vector2i to;
    std::optional<sf::Vector2i> jump;
};

struct Exchange
{
    Move move;
    Tile jumpedPiece;
    Piece movedPiece;
};

std::vector<Move> moves(const GameState& state);