#include "System.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Game.hpp"

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

int System::run()
{
    auto game = ::game();
    for (auto window = ::window(); window->isOpen(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                const auto mousPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                const auto undo = event.mouseButton.button == sf::Mouse::Right;
                game.update(mousPosition, undo);
            }
        }
        window->clear();
        window->draw(game.drawable());
    }
    return 0;
}