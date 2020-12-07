#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "BoardView.hpp"
#include "GameState.hpp"

struct Game
{
    Game(sf::Vector2f viewSize);

    PieceColor currentPlayer() const;

    const Board& board() const;

    GameState& state();

    const GameState& state() const;

    void update(sf::Vector2i tileClicked);

    void update(sf::Vector2f mouseClickPosition, bool undo);

    bool isOver() const;

    const sf::Drawable& drawable() const;

    const BoardView& boardView() const;

private:
    void drawAvailableMoves();

    GameState gameState_;
    BoardView boardView_;

    std::vector<Exchange> lastMoves_;
    std::optional<sf::Vector2i> lastChosedTile_;
};