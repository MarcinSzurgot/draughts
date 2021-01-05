#include "ArgumentParser.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <variant>

#include "Piece.hpp"
#include "SwitchException.hpp"

using namespace std::literals;

namespace
{

std::optional<int> integer(std::string_view string)
{
    const auto first = begin(string);
    const auto last = end(string);
    const auto nonDigit = std::find_if_not(first, last, isdigit);
    if (nonDigit != last)
    {
        return std::nullopt;
    }

    return std::accumulate(first, last, 0, [&](int sumator, char value){
        return sumator * 10 + value - '0';
    });
}

std::optional<PlayerConfig> playerConfig(std::string_view argument)
{
    constexpr auto randomOption = "random"sv;
    constexpr auto humanOption = "human"sv;
    constexpr auto minmaxOption = "minmax-"sv;

    if (argument == humanOption)
    {
        return PlayerConfig{PlayerType::human};
    }
    else if (argument == randomOption)
    {
        return PlayerConfig{PlayerType::random};
    }
    else if (argument.starts_with(minmaxOption))
    {
        const auto value = integer(argument.substr(size(minmaxOption)));
        if (not value)
        {
            return std::nullopt;
        }

        return PlayerConfig{PlayerType::minmax, value};
    }
    return std::nullopt;
}

std::pair<std::string_view, std::string_view> blackAndWhiteArguments(std::string_view first, std::string_view second)
{
    constexpr auto blackOption = "--black="sv;
    constexpr auto whiteOption = "--white="sv;
    if (first.starts_with(blackOption) && second.starts_with(whiteOption))
    {
        return {
            first.substr(size(blackOption)),
            second.substr(size(whiteOption))
        };
    }
    else if (first.starts_with(whiteOption) && second.starts_with(blackOption))
    {
        return {
            second.substr(size(blackOption)), 
            first.substr(size(whiteOption))
        };
    }
    return {};
}

}

ArgumentParser::ArgumentParser(int argc, char** argv)
{
    if (argc != 3)
    {
        errorCode_ = ArgumentParserErrorCode::invalidNumberOfArgs;
        std::cerr << "Invalid number of arguments\n";
        return;
    }

    const auto [black, white] = blackAndWhiteArguments(argv[1], argv[2]);
    const auto blackPlayer = playerConfig(black);
    const auto whitePlayer = playerConfig(white);

    if (not blackPlayer || not whitePlayer)
    {
        errorCode_ = ArgumentParserErrorCode::invalidArgument;
        std::cerr << "Invalid arguments.\n";
    }
    else
    {
        errorCode_ = ArgumentParserErrorCode::success;
        players_ = Players{
            .whitePlayerConfig = *whitePlayer, 
            .blackPlayerConfig = *blackPlayer
        };
    }
}

std::optional<Players> ArgumentParser::players() const
{
    return players_;
}

bool ArgumentParser::success() const
{
    return errorCode() == 0;
}

int ArgumentParser::errorCode() const
{
    return static_cast<std::underlying_type_t<ArgumentParserErrorCode>>(errorCode_);
}