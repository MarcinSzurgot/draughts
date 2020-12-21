#include "System.hpp"

int main(int argc, char** argv)
{
    return System().run({argv, argv + argc});
}