#pragma once

#include <optional>

enum class PlayerType { human, random, minmax };

struct PlayerConfig
{
    PlayerType type;
    std::optional<int> minmaxDepth;
};

struct Players
{
    PlayerConfig whitePlayerConfig;
    PlayerConfig blackPlayerConfig;
};

enum class ArgumentParserErrorCode : int
{
    success = 0,
    invalidNumberOfArgs = 1,
    invalidArgument = 2,
};

struct ArgumentParser
{
    ArgumentParser(int argc, char** argv);

    std::optional<Players> players() const;

    bool success() const;

    int errorCode() const;

private:
    std::optional<Players> players_;
    ArgumentParserErrorCode errorCode_;
};