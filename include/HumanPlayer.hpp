#pragma once

#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include "Game.hpp"
#include "Player.hpp"

struct HumanPlayer : Player
{
    void onEvent(sf::Event event);
    void move(Game& game) override;

private:
    std::vector<sf::Event> eventsToProcess_;
};