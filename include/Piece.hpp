#pragma once

#include <optional>

enum class PieceClass { Man, King };
enum class PieceColor { Black, White };

struct Piece
{
    PieceClass pieceClass;
    PieceColor pieceColor;
};

using Tile = std::optional<Piece>;