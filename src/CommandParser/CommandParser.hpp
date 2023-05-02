#ifndef COMMANDPARSER_COMMANDPARSER_HPP
#define COMMANDPARSER_COMMANDPARSER_HPP

#include "CommandParser/Command.hpp"
#include <fstream>
#include <regex>
#include <string>

class CommandParser
{
protected:
    std::ifstream commandFile;

    std::string getNextLine();
    Command parseCommandFromString(const std::string &s);
    Command getCommandREGFromMatch(const std::smatch &match);
    Command getCommandCLEFromMatch(const std::smatch &match);

public:
    CommandParser(std::string _fileName);
    ~CommandParser();

    Command readNextCommand();

    bool canContinueReading();
};

#endif  // !COMMANDPARSER_COMMANDPARSER_HPP
