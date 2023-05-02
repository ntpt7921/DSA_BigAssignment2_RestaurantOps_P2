#include "CommandParser/CommandParser.hpp"
#include "CommandParser/Command.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

CommandParser::CommandParser(std::string _fileName) : commandFile(_fileName, std::ios::in)
{
    if (!commandFile.is_open())
        throw "CommandReader::constructor(std::string) - "
              "Fail to open file with provided name";
}

CommandParser::~CommandParser() { commandFile.close(); }

std::string CommandParser::getNextLine()
{
    std::string nextLine;
    // if can not read any more (should have check for this)
    // the return value is a empty string (which will be parse as INVALID command)
    if (this->canContinueReading())
        std::getline(commandFile, nextLine);

    // remove all new line char within the string (/r or /n)
    auto findAt = nextLine.find_first_of("\r\n");
    if (findAt != std::string::npos)
        return nextLine.erase(findAt);
    else
        return nextLine;
}

Command CommandParser::parseCommandFromString(const std::string &s)
{
    static const std::regex commandREG("^REG ([a-zA-z]+)$",
                                       std::regex::ECMAScript | std::regex::optimize);
    static const std::regex commandCLE("^CLE ([+-]?\\d+)$",
                                       std::regex::ECMAScript | std::regex::optimize);
    static const std::regex commandPrintHT("^PrintHT$",
                                           std::regex::ECMAScript | std::regex::optimize);
    static const std::regex commandPrintAVL("^PrintAVL$",
                                            std::regex::ECMAScript | std::regex::optimize);
    static const std::regex commandPrintMH("^PrintMH$",
                                           std::regex::ECMAScript | std::regex::optimize);

    std::smatch match;
    try
    {
        if (std::regex_match(s, match, commandREG))
            return getCommandREGFromMatch(match);
        else if (std::regex_match(s, match, commandCLE))
            return getCommandCLEFromMatch(match);
        else if (std::regex_match(s, match, commandPrintHT))
            return Command{Command::CommandType::PrintHT, "", 0};
        else if (std::regex_match(s, match, commandPrintAVL))
            return Command{Command::CommandType::PrintAVL, "", 0};
        else if (std::regex_match(s, match, commandPrintMH))
            return Command{Command::CommandType::PrintMH, "", 0};
    }
    catch (const std::regex_error &ex)
    {
        std::cerr << ex.what() << '\n';
        // do nothing, the last line of method will return a invalid command
    }

    return Command{Command::CommandType::INVALID, "", 0};
}

Command CommandParser::getCommandREGFromMatch(const std::smatch &match)
{
    if (!match.ready())
        return Command{Command::CommandType::INVALID, "", 0};

    return Command{Command::CommandType::REG, match.str(1), 0};
}

Command CommandParser::getCommandCLEFromMatch(const std::smatch &match)
{
    if (!match.ready())
        return Command{Command::CommandType::INVALID, "", 0};

    Command::NumType num = 0;
    try
    {
        num = std::stoi(match.str(1));
    }
    catch (const std::invalid_argument &ex)
    {
        std::cerr << ex.what() << '\n';
        return Command{Command::CommandType::INVALID, "", 0};
    }
    catch (const std::out_of_range &ex)
    {
        std::cerr << ex.what() << '\n';
        return Command{Command::CommandType::INVALID, "", 0};
    }

    return Command{Command::CommandType::CLE, "", num};
}

Command CommandParser::readNextCommand()
{
    std::string line = getNextLine();
    Command newCommand = parseCommandFromString(line);
    newCommand.performSelfCheck();
    return newCommand;
}

bool CommandParser::canContinueReading()
{
    return commandFile.good();  // no EOF or error has happened
}
