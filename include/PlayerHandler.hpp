#pragma once

#include <memory>

#include <SFML/Window/Event.hpp>

#include "ArgumentParser.hpp"
#include "Game.hpp"
#include "Piece.hpp"
#include "Player.hpp"

struct PlayerHandler
{
    PlayerHandler(PlayerType whitePlayerType, PlayerType blackPlayerType, Game& game);

    void onEvent(sf::Event event);

    void move();

private:
    std::reference_wrapper<Game> game_;
    std::shared_ptr<Player> whitePlayer_;
    std::shared_ptr<Player> blackPlayer_;
};