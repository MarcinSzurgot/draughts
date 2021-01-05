#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "ArgumentParser.hpp"
#include "Piece.hpp"
#include "Player.hpp"

struct GameHandler
{
    GameHandler(PlayerConfig whitePlayerConfig,
                PlayerConfig blackPlayerConfig,
                sf::Vector2f gameWindowSize);

    void onEvent(sf::Event event);

    void move();

    bool isOver() const;

    const sf::Drawable& drawable() const;

private:
    Game game_;
    std::shared_ptr<Player> whitePlayer_;
    std::shared_ptr<Player> blackPlayer_;
};