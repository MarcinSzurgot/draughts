#include <chrono>
#include <random>

#include <SFML/Graphics.hpp>

#include "Game.hpp"
#include "GameState.hpp"

namespace
{

std::unique_ptr<sf::RenderWindow> window(sf::Vector2u windowSize)
{
    const auto windowName = "draughts";
    const auto videoMode  = sf::VideoMode(windowSize.x, windowSize.y);
    const auto framerate  = 60;

    auto window = std::make_unique<sf::RenderWindow>(videoMode, windowName);
    window->setFramerateLimit(framerate);
    return window;
}

}

template<typename Container>
auto randomElement(const Container& container, std::mt19937& generator)
{
    auto distribution = std::uniform_int_distribution<int>(0, size(container) - 1);
    return std::next(begin(container), distribution(generator));
}

std::optional<Move> randomMove(const GameState& state)
{
    if (const auto& moves = state.moves(); not empty(moves))
    {
        const auto now = std::chrono::high_resolution_clock::now();
        auto generator = std::mt19937(now.time_since_epoch().count());
        return *randomElement(moves, generator);
    }
    return std::nullopt;
}

int main()
{
    const auto windowSize = sf::Vector2u(600, 600);

    auto game = Game((sf::Vector2f) windowSize);
    auto humanPlayer = PieceColor::White;
    for (auto window = ::window(windowSize); window->isOpen() && not game.isOver(); window->display())
    {
        for (auto event = sf::Event(); window->pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    game.update(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)), false);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    game.update(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)), true);
                }
            }
        }
        window->clear();
        window->draw(game.drawable());
    }
    return 0;
}