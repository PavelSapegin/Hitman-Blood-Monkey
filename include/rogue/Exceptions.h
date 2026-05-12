#pragma once
#include <stdexcept>
#include <string>

namespace rogue
{
    class GameException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error; // Inherit constructors
    };

    class RendererException : public GameException
    {
    public:
        explicit RendererException(const std::string& message) : GameException(message) {}
    };

    class MapException : public GameException
    {
    public:
        explicit MapException(const std::string& message) : GameException(message) {}
    };

    class InitializationException : public GameException
    {
    public:        explicit InitializationException(const std::string& message) : GameException(message) {}
    };
}
