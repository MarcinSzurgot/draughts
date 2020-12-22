#include "GameHandler.hpp"

#include "HumanPlayer.hpp"
#include "RandomCpuPlayer.hpp"
#include "SwitchException.hpp"

namespace
{

std::shared_ptr<Player> player(PlayerType playerType)
{
    switch (playerType)
    {
        case PlayerType::cpu:   return std::make_shared<RandomCpuPlayer>();
        case PlayerType::human: return std::make_shared<HumanPlayer>();
    }
    throw SwitchException("PlayerType", playerType);
}

}

GameHandler::GameHandler(PlayerType whitePlayerType,
                         PlayerType blackPlayerType,
                         sf::Vector2f gameWindowSize)
: game_(gameWindowSize)
, whitePlayer_(player(whitePlayerType))
, blackPlayer_(player(blackPlayerType))
{

}

void GameHandler::onEvent(sf::Event event)
{
    auto didPlayerEvent = [&](std::shared_ptr<Player> player, PieceColor color)
    {
        if (const auto& human = std::dynamic_pointer_cast<HumanPlayer>(player))
        {
            if (game_.currentPlayer() == color)
            {
                human->onEvent(event);
            }
        }
    };

    didPlayerEvent(whitePlayer_, PieceColor::White);
    didPlayerEvent(blackPlayer_, PieceColor::Black);
}

void GameHandler::move()
{
    const auto& player = [&]{
        switch (game_.currentPlayer())
        {
            case PieceColor::White: return whitePlayer_;
            case PieceColor::Black: return blackPlayer_;
        }
        throw SwitchException("PieceColor", game_.currentPlayer());
    } ();

    player->move(game_);
}

bool GameHandler::isOver() const
{
    return game_.isOver();
}

const sf::Drawable& GameHandler::drawable() const
{
    return game_.drawable();
}