#include "Board.hpp"

#include "SwitchException.hpp"

Board::Board(sf::Vector2i size): size_(size), tiles_(size.x * size.y)
{
}

sf::Vector2i Board::size() const { return size_; }

const Tile& Board::operator[](sf::Vector2i position) const
{
    return tiles_[position.y * size_.x + position.x];
}

Tile& Board::operator[](sf::Vector2i position)
{
    return tiles_[position.y * size_.x + position.x];
}