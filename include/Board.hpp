#pragma once

#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

#include "Move.hpp"
#include "Piece.hpp"
#include "Vector2.hpp"

struct Board
{
    Board(sf::Vector2i size);

    Tile& operator[](sf::Vector2i position);

    const Tile& operator[](sf::Vector2i position) const;

    sf::Vector2i size() const;

private:
    sf::Vector2i size_;
    std::vector<Tile> tiles_;
};