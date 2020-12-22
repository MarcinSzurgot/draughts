#pragma once

#include <string>
#include <vector>

enum PlayerType { cpu, human };

struct Players
{
    PlayerType whitePlayerType;
    PlayerType blackPlayerType;
};

enum class ArgumentParserErrorCode : int
{
    success = 0,
    invalidNumberOfArgs = 1,
    invalidArgument = 2,
};

struct ArgumentParser
{
    ArgumentParser(const std::vector<std::string>& args);

    Players players() const;

    bool success() const;

    int errorCode() const;

private:
    std::string gameType_;
    ArgumentParserErrorCode errorCode_;
};