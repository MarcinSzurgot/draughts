#include "System.hpp"

#include <chrono>
#include <iostream>
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

int System::run(int argc, char** argv)
{
    const auto startTime = std::chrono::high_resolution_clock::now();

    const auto argParser = ArgumentParser(argc, argv);
    if (not argParser.success())
    {
        return argParser.errorCode();
    }

    const auto players = argParser.players();
    auto gameHandler = GameHandler(
        players->whitePlayerConfig,
        players->blackPlayerConfig,
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

    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Elapsed time: " << elapsed.count() << "ms.\n";
    return 0;
}