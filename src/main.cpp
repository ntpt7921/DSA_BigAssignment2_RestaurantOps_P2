#include "main.h"
#include "AVL/AVLTree.hpp"
#include "Queue/QueueFIFO.hpp"
#include "Queue/QueueLFCO.hpp"
#include "Queue/QueueLRCO.hpp"
#include "Queue/TestQueue.hpp"

int main()
{
    /* std::string fileName = "test.txt"; */
    /* simulate(fileName); */

    TestQueue::QueueLFCO::test();

    return 0;
}
