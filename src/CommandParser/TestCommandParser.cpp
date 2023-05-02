#include "CommandParser/TestCommandParser.hpp"
#include "CommandParser/Command.hpp"
#include "CommandParser/CommandParser.hpp"
#include <iostream>

void TestCommandParser::testWithFile(const std::string &fileName)
{
    auto printCommand = [](const Command &cmd)
    {
        switch (cmd.type)
        {
            case Command::CommandType::REG:
                std::cerr << "REG";
                break;
            case Command::CommandType::CLE:
                std::cerr << "CLE";
                break;
            case Command::CommandType::PrintHT:
                std::cerr << "PrintHT";
                break;
            case Command::CommandType::PrintAVL:
                std::cerr << "PrintAVL";
                break;
            case Command::CommandType::PrintMH:
                std::cerr << "PrintMH";
                break;
            case Command::CommandType::INVALID:
                std::cerr << "INVALID";
                break;
            default:
                throw "Unknown command value";
        }
        std::cerr << ' ' << cmd.num << ' ' << cmd.name << '\n';
    };

    CommandParser parser(fileName);
    while (parser.canContinueReading())
    {
        Command cmd = parser.readNextCommand();
        printCommand(cmd);
    }
}
