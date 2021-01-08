#include "GameState.hpp"

#include "SwitchException.hpp"

namespace
{

Piece pieceAfterMove(const Piece& piece,
                     sf::Vector2i positionAfterMove,
                     sf::Vector2i boardSize)
{
    const auto promotes = [&] {
        switch (piece.pieceColor)
        {
            case PieceColor::Black: return positionAfterMove.y == boardSize.y - 1;
            case PieceColor::White: return positionAfterMove.y == 0;
        }
        throw SwitchException("PieceColor", piece.pieceColor);
    } ();
    return {promotes ? PieceClass::King : piece.pieceClass, piece.pieceColor};
}

PieceColor toggled(PieceColor color)
{
    switch (color)
    {
        case PieceColor::White: return PieceColor::Black;
        case PieceColor::Black: return PieceColor::White;
    }
    throw SwitchException("PieceColor", color);
}

}

GameState::GameState(Board board, PieceColor currentPlayer)
: needsUpdate_(true),
  board_(std::move(board)),
  currentPlayer_(currentPlayer)
{

}

const Board& GameState::board() const
{
    return board_;
}

std::span<const Move> GameState::moves() const
{
    if (needsUpdate_)
    {
        currentPlayerMoves_ = ::moves(*this);
        needsUpdate_ = false;
    }
    return std::span(begin(currentPlayerMoves_), end(currentPlayerMoves_));
}

std::span<const Move> GameState::moves(sf::Vector2i position) const
{
    const auto moves = this->moves();
    const auto pieceOnPosition = [position](const Move& move){
        return move.from == position;
    };
    const auto first = std::find_if(begin(moves), end(moves), pieceOnPosition);
    const auto last  = std::find_if_not(first, end(moves), pieceOnPosition);
    return {first, last};
}

Exchange GameState::move(const Move& move)
{
    const auto exchange = Exchange{
        move,
        move.jump ? *board_[*move.jump] : Tile(),
        *board_[move.from]
    };

    currentPlayer_ = toggled(currentPlayer_);

    board_[move.to] = pieceAfterMove(exchange.movedPiece, move.to, board_.size());
    board_[move.from] = Tile();
    if (move.jump)
    {
        board_[*move.jump] = Tile();
    }

    needsUpdate_ = true;

    return exchange;
}

void GameState::undo(const Exchange& exchange)
{
    board_[exchange.move.to] = Tile();
    board_[exchange.move.from] = exchange.movedPiece;
    if (const auto& jump = exchange.move.jump)
    {
        board_[*jump] = exchange.jumpedPiece;
    }
    currentPlayer_ = toggled(currentPlayer_);
    needsUpdate_ = true;
}

PieceColor GameState::currentPlayer() const
{
    return currentPlayer_;
}