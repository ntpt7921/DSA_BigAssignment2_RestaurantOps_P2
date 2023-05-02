#include "CommandParser/Command.hpp"
#include "main.h"
#include <cctype>
#include <string>

void Command::performSelfCheck()
{
    if (this->type == CommandType::REG)
    {
        // name is a non-empty string without whitespace, consists of alphabet char (normal and
        // capital), any other string is not a valid name

        if (this->name.size() == 0)
        {
            this->type = CommandType::INVALID;
            return;
        }

        for (char c : this->name)
        {
            if (!std::isalpha(c))
            {
                this->type = CommandType::INVALID;
                return;
            }
        }
    }
    else if (this->type == CommandType::CLE)
    {
        // num must be within range [0, +inf] (within the limit of binary representation)
        return;
    }

    return;
}

bool Command::isValid() { return (this->type != CommandType::INVALID); }
