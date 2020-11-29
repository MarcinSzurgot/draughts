#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

struct SwitchException : std::invalid_argument
{
    template<typename Enum>
    SwitchException(const std::string& enumName, Enum invalidEnumValue)
    : std::invalid_argument(
        std::string("An invalid value for enum: ")
        + enumName
        + ", underlying integer value: "
        + std::to_string(static_cast<std::underlying_type_t<Enum>>(invalidEnumValue)))
    {

    }
};