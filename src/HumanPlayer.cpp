#include "HumanPlayer.hpp"

namespace
{

void move(Game& game, sf::Event::MouseButtonEvent mouseButton)
{
    const auto mousPosition = sf::Vector2f(mouseButton.x, mouseButton.y);
    const auto undo = mouseButton.button == sf::Mouse::Right;
    game.update(mousPosition, undo);
}

}

void HumanPlayer::onEvent(sf::Event event)
{
    eventsToProcess_.push_back(event);
}

void HumanPlayer::move(Game& game)
{
    for (const auto& event : eventsToProcess_)
    {
        switch (event.type)
        {
            case sf::Event::MouseButtonReleased:
                ::move(game, event.mouseButton);
                break;
            default:
                break;
        }
    }
    eventsToProcess_.clear();
}
