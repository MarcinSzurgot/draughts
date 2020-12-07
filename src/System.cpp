#include "System.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Game.hpp"
#include "RandomCpuPlayer.hpp"

namespace
{
    enum Player { cpu, human };

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
    if (size(parameters) != 2)
    {
        std::cerr << "Not enough parameters.\n";
        return 1;
    }

    const auto& gameType = parameters[1];

    if (gameType != "pvp"
        && gameType != "pvc"
        && gameType != "cvp"
        && gameType != "cvc")
    {
        std::cerr << "Invalid parameters.\n";
        return 2;
    }

    const auto playerOne = gameType.starts_with("p") ? Player::human : Player::cpu;
    const auto playerTwo = gameType.ends_with("p")   ? Player::human : Player::cpu;

    auto cpuPlayer = RandomCpuPlayer();

    auto game = ::game();
    for (auto window = ::window(); window->isOpen() && not game.isOver(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if ((playerOne == Player::human && game.currentPlayer() == PieceColor::White)
                    || (playerTwo == Player::human && game.currentPlayer() == PieceColor::Black))
                {
                    const auto mousPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                    const auto undo = event.mouseButton.button == sf::Mouse::Right;
                    game.update(mousPosition, undo);
                }
            }
        }

        if ((playerOne == Player::cpu && game.currentPlayer() == PieceColor::White)
            || (playerTwo == Player::cpu && game.currentPlayer() == PieceColor::Black))
        {
            cpuPlayer.update(game);
        }

        window->clear();
        window->draw(game.boardView());
    }
    return 0;
}