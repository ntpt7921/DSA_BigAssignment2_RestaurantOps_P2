#include "main.h"
#include "AVL/AVLTree.hpp"
#include "HashTable/StaticHashTable.hpp"
#include "HuffmanCoding/HuffmanTreeWrapper.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"

#include "HuffmanCoding/TestHuffmanCoding.hpp"
#include "Queue/TestQueue.hpp"

int main()
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    TestQueue::QueueLFCO::test();
    TestHuffmanCoding::HuffTree::test();
    TestHuffmanCoding::HuffmanTreeWrapper::test();

    return 0;
}
