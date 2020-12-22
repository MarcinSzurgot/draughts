#include "ArgumentParser.hpp"

#include <iostream>

#include "SwitchException.hpp"

namespace
{

PlayerType playerType(char playerSymbol)
{
    switch (playerSymbol)
    {
        case 'p': return PlayerType::human;
        case 'c': return PlayerType::cpu;
    }
    throw std::invalid_argument(std::string("Unrecognized player symbol: \"") + playerSymbol + "\"");
}

ArgumentParserErrorCode errorCode(const std::vector<std::string>& args)
{
    if (size(args) != 2)
    {
        return ArgumentParserErrorCode::invalidNumberOfArgs;
    }

    if (args[1] != "pvp"
        && args[1] != "pvc"
        && args[1] != "cvp"
        && args[1] != "cvc")
    {
        return ArgumentParserErrorCode::invalidArgument;
    }

    return ArgumentParserErrorCode::success;
}

std::string errorMessage(ArgumentParserErrorCode errorCode)
{
    switch (errorCode)
    {
        case ArgumentParserErrorCode::invalidNumberOfArgs: return "Not enough parameters.";
        case ArgumentParserErrorCode::invalidArgument:     return "Invalid parameters.";
        case ArgumentParserErrorCode::success:             return "";
    }
    throw SwitchException("ArgumentParserErrorCode", errorCode);
}

}

ArgumentParser::ArgumentParser(const std::vector<std::string>& args)
{
    errorCode_ = ::errorCode(args);
    if (const auto errorMessage = ::errorMessage(errorCode_); not empty(errorMessage))
    {
        std::cerr << errorMessage << '\n';
    }
    else
    {
        gameType_ = args[1];
    }
}

Players ArgumentParser::players() const
{
    return {
        playerType(gameType_[0]),
        playerType(gameType_[2])
    };
}

bool ArgumentParser::success() const
{
    return errorCode() == 0;
}

int ArgumentParser::errorCode() const
{
    return static_cast<std::underlying_type_t<ArgumentParserErrorCode>>(errorCode_);
}