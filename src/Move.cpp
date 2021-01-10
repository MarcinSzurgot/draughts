#include "Move.hpp"

#include "GameState.hpp"
#include "SwitchException.hpp"
#include "Utility.hpp"

namespace
{

struct PieceIterator
{
    PieceIterator(const Board& board, PieceColor currentPlayer)
    : current_(board.size()),
      last_(board.size(), board.size()),
      board_(board),
      currentPlayer_(currentPlayer)
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
        for (; current_ != last_ 
               && (not board_[*current_] 
                   || board_[*current_]->pieceColor != currentPlayer_); 
               ++current_);
    }

    MatrixIterator<int> current_;
    MatrixIterator<int> last_;
    const Board& board_;
    PieceColor currentPlayer_;
};

bool contains(sf::Vector2i size, sf::Vector2i position)
{
    const auto [x, y] = position;
    return x >= 0 && x < size.x && y >= 0 && y < size.y;
}

std::vector<sf::Vector2i> directions(const Piece& piece)
{
    static const auto white = std::vector{
        sf::Vector2i{-1, -1},
        sf::Vector2i{ 1, -1},
    };

    static const auto black = std::vector{
        sf::Vector2i{-1, 1},
        sf::Vector2i{ 1, 1},
    };

    static const auto king = std::vector{
        sf::Vector2i{-1, -1},
        sf::Vector2i{ 1, -1},
        sf::Vector2i{-1,  1},
        sf::Vector2i{ 1,  1},
    };

    switch (piece.pieceClass)
    {
        case PieceClass::Man:
            switch (piece.pieceColor)
            {
                case PieceColor::White: return white;
                case PieceColor::Black: return black;
            }
            throw SwitchException("PieceColor", piece.pieceColor);
        case PieceClass::King: return king;
    }
    throw SwitchException("PieceClass", piece.pieceClass);
}

int distance(const Piece& piece)
{
    switch (piece.pieceClass)
    {
        case PieceClass::Man:  return 1;
        case PieceClass::King: return std::numeric_limits<int>::max();
    }
    throw SwitchException("PieceClass", piece.pieceClass);
}

template<typename UnaryOperator>
void getMoves (const GameState& state, UnaryOperator&& unaryOperator)
{
    for (const auto& [position, piece] : PieceIterator(state.board(), state.currentPlayer()))
    {
        const auto distance = ::distance(piece);
        const auto& directions = ::directions(piece);
        for (const auto& direction : directions)
        {
            auto lastJump = std::optional<sf::Vector2i>();
            for (auto d = 1; (d - lastJump.has_value()) <= distance; ++d)
            {
                const auto destination = position + direction * d;
                if (not contains(state.board().size(), destination))
                {
                    break;
                }
                else if (const auto otherPawn = state.board()[destination])
                {
                    if (lastJump || otherPawn->pieceColor == piece.pieceColor)
                    {
                        break;
                    }
                    else
                    {
                        lastJump = destination;
                    }
                }
                else
                {
                    unaryOperator(Move{position, destination, lastJump});
                }
            }
        }
    }
}

}

std::vector<Move> moves(const GameState& state)
{
    auto numberOfMoves = 0;
    auto containsJumps = false;
    getMoves(state, [&](const Move& move){
        if (move.jump && not containsJumps)
        {
            numberOfMoves = 0;
            containsJumps = true;
        }
        numberOfMoves += not containsJumps || (containsJumps && move.jump);
    });
    auto moves = std::vector<Move>();
    moves.reserve(numberOfMoves);
    getMoves(state, [&](const Move& move){
        if (not containsJumps || (containsJumps && move.jump))
        {
            moves.push_back(move);
        }
    });
    return moves;
}