#pragma once

#include <functional>
#include <iostream>

#include <SFML/Graphics.hpp>

template<typename T>
sf::Vector2<T> operator+=(sf::Vector2<T> lhs, sf::Vector2<T> rhs)
{
    return lhs = lhs + rhs;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, sf::Vector2<T> vector)
{
    return os << '{' << vector.x << ", " << vector.y << '}';
}

namespace std
{
    template<typename T>
    struct hash<sf::Vector2<T>>
    {
        std::size_t operator()(const sf::Vector2<T>& vector) const noexcept
        {
            const auto h1 = std::hash<int>{}(vector.x);
            const auto h2 = std::hash<int>{}(vector.y);
            return h1 ^ (h2 << 1);
        }
    };
}