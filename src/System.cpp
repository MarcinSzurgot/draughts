#include "System.hpp"

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "ArgumentParser.hpp"
#include "GameHandler.hpp"

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

}

int System::run(const std::vector<std::string>& parameters)
{
    const auto argParser = ArgumentParser(parameters);
    if (not argParser.success())
    {
        return argParser.errorCode();
    }

    const auto players = argParser.players();
    auto gameHandler = GameHandler(
        players.whitePlayerType,
        players.blackPlayerType,
        {600.f, 600.f}
    );

    for (auto window = ::window(); window->isOpen() && not gameHandler.isOver(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }

            gameHandler.onEvent(event);
        }

        gameHandler.move();

        window->clear();
        window->draw(gameHandler.drawable());
    }
    return 0;
}