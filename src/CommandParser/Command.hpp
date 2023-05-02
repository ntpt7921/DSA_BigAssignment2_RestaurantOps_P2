#ifndef COMMANDPARSER_COMMAND_HPP
#define COMMANDPARSER_COMMAND_HPP

#include <cstdint>
#include <string>

struct Command
{
public:
    enum class CommandType
    {
        REG,
        CLE,
        PrintHT,
        PrintAVL,
        PrintMH,
        INVALID,
    };

    using NumType = int_fast32_t;

public:
    CommandType type;
    std::string name;
    NumType num;

    Command() = default;
    Command(const Command &c) = default;
    ~Command() = default;
    Command &operator=(const Command &c) = default;

    void performSelfCheck();
    bool isValid();
};

#endif  // !COMMANDPARSER_COMMAND_HPP
