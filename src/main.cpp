#include "main.h"

#include "CommandParser/Command.hpp"
#include "CommandParser/CommandParser.hpp"
#include "Restaurant/Restaurant.hpp"
#include <string>

int main(int argc, char *argv[])
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    for (int i = 0; i < argc; i++)
    {
        std::string fileName = argv[i];
        CommandParser reader(fileName);
        Restaurant restaurant;

        while (reader.canContinueReading())
        {
            Command currCommand = reader.readNextCommand();
            restaurant.processCommand(currCommand);
        }
    }

    return 0;
}
