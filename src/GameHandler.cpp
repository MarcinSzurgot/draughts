#include "GameHandler.hpp"

#include "HumanPlayer.hpp"
#include "RandomCpuPlayer.hpp"
#include "SwitchException.hpp"

namespace
{

std::shared_ptr<Player> player(PlayerConfig playerConfig)
{
    switch (playerConfig.type)
    {
        case PlayerType::human:  return std::make_shared<HumanPlayer>();
        case PlayerType::random: return std::make_shared<RandomCpuPlayer>();
        case PlayerType::minmax: return {};
    }
    throw SwitchException("PlayerType", playerConfig.type);
}

}

GameHandler::GameHandler(PlayerConfig whitePlayerConfig,
                         PlayerConfig blackPlayerConfig,
                         sf::Vector2f gameWindowSize)
: game_(gameWindowSize)
, whitePlayer_(player(whitePlayerConfig))
, blackPlayer_(player(blackPlayerConfig))
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