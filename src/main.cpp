#include "main.h"
#include "AVL/AVLTree.hpp"
#include "HashTable/StaticHashTable.hpp"
#include "HashTable/TestHashTable.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"

int main()
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    TestHashTable::StaticHashTable::testAll();

    return 0;
}
