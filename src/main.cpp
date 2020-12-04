#include "System.hpp"

// template<typename Container>
// auto randomElement(const Container& container, std::mt19937& generator)
// {
//     auto distribution = std::uniform_int_distribution<int>(0, size(container) - 1);
//     return std::next(begin(container), distribution(generator));
// }

// std::optional<Move> randomMove(const GameState& state)
// {
//     if (const auto& moves = state.moves(); not empty(moves))
//     {
//         const auto now = std::chrono::high_resolution_clock::now();
//         auto generator = std::mt19937(now.time_since_epoch().count());
//         return *randomElement(moves, generator);
//     }
//     return std::nullopt;
// }

int main()
{
    auto system = System();
    return system.run();
}