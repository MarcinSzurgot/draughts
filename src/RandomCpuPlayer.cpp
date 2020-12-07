#include "RandomCpuPlayer.hpp"

#include <stdexcept>
#include <random>

namespace
{

Move randomElement(std::span<const Move> moves, std::mt19937& generator)
{
    auto distribution = std::uniform_int_distribution<int>(0, size(moves) - 1);
    return *std::next(begin(moves), distribution(generator));
}

std::optional<Move> randomMove(const GameState& state)
{
    if (const auto& moves = state.moves(); not empty(moves))
    {
        const auto now = std::chrono::high_resolution_clock::now();
        auto generator = std::mt19937(now.time_since_epoch().count());
        return randomElement(moves, generator);
    }
    return std::nullopt;
}

}

RandomCpuPlayer::RandomCpuPlayer() : step_(0)
{
}

void RandomCpuPlayer::update(Game& game)
{
    const auto now = std::chrono::high_resolution_clock::now();
    if (step_ == 0)
    {
        currentMove_ = randomMove(game.state());
        if (not currentMove_.has_value())
        {
            return;
        }

        timeDelay_ = now;
        step_ = 1;
    }
    else if (step_ == 1 || step_ == 2)
    {
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeDelay_);
        if (elapsed >= std::chrono::milliseconds(100))
        {
            game.update(step_ == 1 ? currentMove_->from : currentMove_->to);
            timeDelay_ = now;
            step_ = step_ == 1 ? 2 : 0;;
        }
    }
    else
    {
        throw std::invalid_argument("Invalid step value.");
    }
}