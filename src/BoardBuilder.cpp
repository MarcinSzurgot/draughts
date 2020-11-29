#include "BoardBuilder.hpp"

namespace
{

Tile tile(sf::Vector2i size, int row, int col)
{
    const auto freeRows = size.y % 2 ? 3 : 2;
    const auto rowsPerSide = (size.y - freeRows) / 2;
    if ((col + row) % 2 == 1)
    {
        if (row < rowsPerSide)
        {
            return Piece{PieceClass::Man, PieceColor::Black};
        }
        else if (row >= size.y - rowsPerSide)
        {
            return Piece{PieceClass::Man, PieceColor::White};
        }
    }
    return std::nullopt;
}

}

Board BoardBuilder::classicBoard(sf::Vector2i size)
{
    auto board = Board(size);
    for (auto row = 0; row < size.y; ++row)
    {
        for (auto col = 0; col < size.x; ++col)
        {
            board[{col, row}] = ::tile(size, row, col);
        }
    }
    return board;
}