#include "Game.hpp"

#include "BoardBuilder.hpp"

Game::Game(sf::Vector2f viewSize)
: gameState_(BoardBuilder::classicBoard({8, 8}), PieceColor::White),
  boardView_(gameState_.board().size(), viewSize)
{
    boardView_.update(gameState_.board());
    drawAvailableMoves();
}

PieceColor Game::currentPlayer() const
{
    return gameState_.currentPlayer();
}

const Board& Game::board() const { return gameState_.board(); }

GameState& Game::state()
{
    return gameState_;
}

const GameState& Game::state() const
{
    return gameState_;
}

void Game::update(sf::Vector2i tileClicked)
{
    if (tileClicked.x < 0
        || tileClicked.x >= gameState_.board().size().x
        || tileClicked.y < 0
        || tileClicked.y >= gameState_.board().size().y)
    {
        std::cout << tileClicked << '\n';
        throw std::invalid_argument("Clicked tile is out of board");
    }

    if (lastChosedTile_)
    {
        const auto pieceMoves = gameState_.moves(*lastChosedTile_);
        const auto pieceMove = std::find_if(begin(pieceMoves), end(pieceMoves), [&](const Move& move){
            return move.to == tileClicked;
        });
        if (pieceMove != end(pieceMoves))
        {
            gameState_.move(*pieceMove);
            lastChosedTile_ = std::nullopt;
            boardView_.update(gameState_.board());
        }
    }

    lastChosedTile_ = tileClicked;
    drawAvailableMoves();
}

void Game::update(sf::Vector2f mouseClickPosition, bool undo)
{
    if (undo && not empty(lastMoves_))
    {
        gameState_.undo(lastMoves_.back());
        lastMoves_.pop_back();
        boardView_.update(gameState_.board());
        drawAvailableMoves();
        return;
    }
    const auto currentlyChosedTile = boardView_.tile(mouseClickPosition);
    if (not currentlyChosedTile)
    {
        boardView_.drawMovablePieces(gameState_.moves());
    }
    else if (lastChosedTile_)
    {
        const auto pieceMoves = gameState_.moves(*lastChosedTile_);
        const auto pieceMove = std::find_if(begin(pieceMoves), end(pieceMoves), [&](const Move& move){
            return move.to == *currentlyChosedTile;
        });
        if (pieceMove != end(pieceMoves))
        {
            lastMoves_.push_back(gameState_.move(*pieceMove));
            lastChosedTile_ = std::nullopt;
            boardView_.update(gameState_.board());
        }
    }

    lastChosedTile_ = currentlyChosedTile;
    drawAvailableMoves();
}

bool Game::isOver() const
{
    return empty(gameState_.moves());
}

const sf::Drawable& Game::drawable() const
{
    return boardView_;
}

void Game::drawAvailableMoves()
{
    if (lastChosedTile_)
    {
        const auto& chosenMoves = gameState_.moves(*lastChosedTile_);
        if (not empty(chosenMoves))
        {
            boardView_.drawMoves(chosenMoves);
            return;
        }
    }
    boardView_.drawMovablePieces(gameState_.moves());
}