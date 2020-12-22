#include "System.hpp"

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "ArgumentParser.hpp"
#include "Game.hpp"
#include "PlayerHandler.hpp"

namespace
{

std::unique_ptr<sf::RenderWindow> window()
{
    const auto windowName = "draughts";
    const auto videoMode  = sf::VideoMode(600, 600);
    const auto framerate  = 60;

    auto window = std::make_unique<sf::RenderWindow>(videoMode, windowName);
    window->setFramerateLimit(framerate);
    return window;
}

Game game()
{
    return Game({600.f, 600.f});
}

}

int System::run(const std::vector<std::string>& parameters)
{
    const auto argParser = ArgumentParser(parameters);
    if (not argParser.success())
    {
        return argParser.errorCode();
    }

    const auto players = argParser.players();
    auto game = ::game();
    auto playerHandler = PlayerHandler(players.whitePlayerType, players.blackPlayerType, game);

    for (auto window = ::window(); window->isOpen() && not game.isOver(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }

            playerHandler.onEvent(event);
        }

        playerHandler.move();

        window->clear();
        window->draw(game.boardView());
    }
    return 0;
}