#pragma once

#include <iostream>

#include <SFML/System/Vector2.hpp>

template<typename Integer>
struct MatrixRange;

template<typename Integer>
struct MatrixIterator
{
    friend class MatrixRange<Integer>;

    using value_type = sf::Vector2<Integer>;

    MatrixIterator(value_type size)
    : columnRange_(0, size.x),
      current_(0, 0)
    {

    }

    MatrixIterator(value_type current, value_type size)
    : columnRange_(0, size.x),
      current_(current)
    {

    }

    friend MatrixIterator& operator++(MatrixIterator& iterator)
    {
        iterator.current_.x++;
        if (iterator.current_.x == iterator.columnRange_.y)
        {
            iterator.current_.x = iterator.columnRange_.x;
            iterator.current_.y++;
        }
        return iterator;
    }

    friend value_type operator*(const MatrixIterator& iterator)
    {
        return iterator.current_;
    }

    friend bool operator!=(const MatrixIterator& lhs, const MatrixIterator& rhs)
    {
        return lhs.current_.y != rhs.current_.y;
    }

private:
    sf::Vector2<Integer> columnRange_;
    sf::Vector2<Integer> current_;
};

namespace sf
{

template<typename Integer>
MatrixIterator<Integer> begin(sf::Vector2<Integer> size)
{
    return {size};
}

template<typename Integer>
MatrixIterator<Integer> end(sf::Vector2<Integer> size)
{
    return {size, size};
}

}

template<typename Container, typename Comparator>
auto find(Container&& container, Comparator&& comparator)
{
    const auto first = begin(container);
    const auto last  = end(container);
    const auto found = std::find_if(first, last, std::forward<Comparator>(comparator));

    if (found != last)
    {
        return std::make_optional(*found);
    }
    return std::optional<typename decltype(found)::value_type>();
}