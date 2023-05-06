#include "main.h"
#include "AVL/AVLTree.hpp"
#include "HashTable/StaticHashTable.hpp"
#include "HuffmanCoding/HuffmanTreeWrapper.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"

#include "AVL/TestAVL.hpp"
#include "HashTable/TestHashTable.hpp"
#include "Queue/TestQueue.hpp"

int main()
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    AVLTest::printTest();
    TestHashTable::StaticHashTable::testPrint();
    TestQueue::QueueLFCO::testPrint();

    return 0;
}
