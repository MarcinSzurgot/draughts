#pragma once

#include <span>

#include "Board.hpp"

struct GameState
{
    GameState(Board board, PieceColor currentPlayer);

    const Board& board() const;

    std::span<const Move> moves() const;

    std::span<const Move> moves(sf::Vector2i position) const;

    Exchange move(const Move& move);

    void undo(const Exchange& exchange);

    PieceColor currentPlayer() const;

private:
    mutable bool needsUpdate_;
    mutable std::vector<Move> currentPlayerMoves_;

    Board board_;
    PieceColor currentPlayer_;
};