#include "MinMaxPlayer.hpp"

#include <iostream>
#include <limits>
#include <numeric>
#include <string>

#include "Utility.hpp"

using namespace std::literals;

namespace
{

struct PieceIterator
{
    PieceIterator(const Board& board)
    : current_(board.size()),
      last_(board.size(), board.size()),
      board_(board)
    {
        findPiece();
    }

    friend PieceIterator& begin(PieceIterator& iterator)
    {
        return iterator;
    }

    friend PieceIterator& end(PieceIterator& iterator)
    {
        return iterator;
    }

    friend PieceIterator& operator++(PieceIterator& iterator)
    {
        ++iterator.current_;
        iterator.findPiece();
        return iterator;
    }

    friend std::pair<sf::Vector2i, const Piece&> operator*(const PieceIterator& iterator)
    {
        return {*iterator.current_, *iterator.board_[*iterator.current_]};
    }

    friend bool operator!=(const PieceIterator& lhs, const PieceIterator& rhs)
    {
        return lhs.current_ != rhs.last_;
    }

private:
    void findPiece()
    {
        for (; current_ != last_ && not board_[*current_]; ++current_);
    }

    MatrixIterator<int> current_;
    MatrixIterator<int> last_;
    const Board& board_;
};

std::span<const Move> movesForPosition(const std::vector<Move>& moves, sf::Vector2i position)
{
    const auto pieceOnPosition = [position](const Move& move){
        return move.from == position;
    };
    const auto first = std::find_if(begin(moves), end(moves), pieceOnPosition);
    const auto last  = std::find_if_not(first, end(moves), pieceOnPosition);
    return {first, last};
}

int evaluate(const GameState& state)
{
    return std::accumulate(
            PieceIterator(state.board()), 
            PieceIterator(state.board()), 
            0,
            [&](int sumator, const auto& piece){
        return sumator 
        - (state.currentPlayer() == piece.second.pieceColor)
        + (state.currentPlayer() != piece.second.pieceColor);
    });
}

int minmax(GameState& state, int depth, bool maximizingPlayer)
{
    if (depth == 0)
    {
        return evaluate(state);
    }

    auto initialized = false;
    auto best = maximizingPlayer ? std::numeric_limits<int>::lowest() : std::numeric_limits<int>::max();
    const auto allMoves = state.vectorMoves();
    for (const auto& position : state.board().size())
    {
        for (const auto& move : movesForPosition(allMoves, position))
        {
            const auto exchange = state.move(move);
            const auto eval = minmax(state, depth - 1, !maximizingPlayer);
            best = maximizingPlayer ? std::max(best, eval) : std::min(best, eval);
            initialized = true;
            state.undo(exchange);
        }
    }

    if (not initialized)
    {
        return evaluate(state);
    }

    return best;
}

std::optional<Move> minmax(GameState state, int depth)
{
    const auto board = state.board();
    auto bestEval = std::numeric_limits<int>::lowest();
    auto bestMove = std::optional<Move>();
    const auto spanMoves = state.moves();
    const auto allMoves = std::vector<Move>(begin(spanMoves), end(spanMoves));
    for (const auto& [position, piece] : PieceIterator(state.board()))
    {
        for (const auto& move : movesForPosition(allMoves, position))
        {
            const auto exchange = state.move(move);
            const auto eval = minmax(state, depth, false);
            if (eval > bestEval)
            {
                bestEval = eval;
                bestMove = move;
            }
            state.undo(exchange);
        }
    }
    return bestMove;
}

}

MinMaxPlayer::MinMaxPlayer(int maxDepth) : maxDepth_(maxDepth), step_(0)
{

}

void MinMaxPlayer::move(Game& game)
{
    const auto now = std::chrono::high_resolution_clock::now();
    if (step_ == 0)
    {
        currentMove_ = minmax(game.state(), maxDepth_);
        if (not currentMove_.has_value())
        {
            return;
        }

        timeDelay_ = now;
        step_ = 1;
    }
    else if (step_ == 1 || step_ == 2)
    {
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeDelay_);
        if (elapsed >= std::chrono::milliseconds(100))
        {
            game.update(step_ == 1 ? currentMove_->from : currentMove_->to);
            timeDelay_ = now;
            step_ = step_ == 1 ? 2 : 0;;
        }
    }
    else
    {
        throw std::invalid_argument("Invalid step value: "s + std::to_string(step_));
    }
}