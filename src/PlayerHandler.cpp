#include "PlayerHandler.hpp"

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

PlayerHandler::PlayerHandler(PlayerType whitePlayerType, PlayerType blackPlayerType, Game& game)
: game_(game)
, whitePlayer_(player(whitePlayerType))
, blackPlayer_(player(blackPlayerType))
{

}

void PlayerHandler::onEvent(sf::Event event)
{
    auto didPlayerEvent = [&](std::shared_ptr<Player> player, PieceColor color)
    {
        if (const auto& human = std::dynamic_pointer_cast<HumanPlayer>(player))
        {
            if (game_.get().currentPlayer() == color)
            {
                human->onEvent(event);
            }
        }
    };

    didPlayerEvent(whitePlayer_, PieceColor::White);
    didPlayerEvent(blackPlayer_, PieceColor::Black);
}

void PlayerHandler::move()
{
    const auto& player = [&]{
        switch (game_.get().currentPlayer())
        {
            case PieceColor::White: return whitePlayer_;
            case PieceColor::Black: return blackPlayer_;
        }
        throw SwitchException("PieceColor", game_.get().currentPlayer());
    } ();

    player->move(game_);
}