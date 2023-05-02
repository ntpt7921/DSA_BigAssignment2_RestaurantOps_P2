#include "main.h"
#include "AVL/AVLTree.hpp"
#include "HashTable/StaticHashTable.hpp"
#include "HuffmanCoding/HuffmanTreeWrapper.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"

#include "CommandParser/TestCommandParser.hpp"

int main(int argc, char *argv[])
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    for (int i = 1; i < argc; i++)
        TestCommandParser::testWithFile(std::string(argv[i]));

    return 0;
}
