#include "System.hpp"

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Game.hpp"
#include "HumanPlayer.hpp"
#include "RandomCpuPlayer.hpp"
#include "SwitchException.hpp"

namespace
{

enum PlayerType { cpu, human };

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

PlayerType playerType(char playerSymbol)
{
    switch (playerSymbol)
    {
        case 'p': return PlayerType::human;
        case 'c': return PlayerType::cpu;
    }
    throw std::invalid_argument(std::string("Unrecognized player symbol: \"") + playerSymbol + "\"");
}

struct PlayerFactory
{
    static std::shared_ptr<Player> player(PlayerType playerType)
    {
        switch (playerType)
        {
            case PlayerType::cpu:   return std::make_shared<RandomCpuPlayer>();
            case PlayerType::human: return std::make_shared<HumanPlayer>();
        }
        throw SwitchException("PlayerType", playerType);
    }
};

std::vector<std::pair<PieceColor, std::shared_ptr<Player>>> players(std::string_view gameType)
{
    return {
        {PieceColor::White, PlayerFactory::player(playerType(gameType[0]))},
        {PieceColor::Black, PlayerFactory::player(playerType(gameType[2]))}
    };
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

    auto players = ::players(gameType);
    auto game = ::game();

    for (auto window = ::window(); window->isOpen() && not game.isOver(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }

            for (auto& [playerColor, player] : players)
            {
                auto humanPlayer = std::dynamic_pointer_cast<HumanPlayer>(player);
                if (humanPlayer && playerColor == game.currentPlayer())
                {
                    humanPlayer->onEvent(event);
                }
            }
        }

        for (auto& [playerColor, player] : players)
        {
            if (playerColor == game.currentPlayer())
            {
                player->move(game);
            }
        }

        window->clear();
        window->draw(game.boardView());
    }
    return 0;
}