#include "System.hpp"

int main(int argc, char** argv)
{
    auto system = System();
    return system.run({argv, argv + argc});
}