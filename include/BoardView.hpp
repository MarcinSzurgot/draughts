#pragma once

#include <span>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "Move.hpp"

struct BoardView : sf::Drawable, sf::Transformable
{
    BoardView(sf::Vector2i boardSize, sf::Vector2f viewSize);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(const Board& board);

    void drawMoves(std::span<const Move> moves);

    void drawMovablePieces(std::span<const Move> moves);

    std::optional<sf::Vector2i> tile(sf::Vector2f mousePos) const;

private:
    sf::Vector2i size_;
    std::vector<sf::RectangleShape> tiles_;
    std::vector<sf::CircleShape> pieces_;
};