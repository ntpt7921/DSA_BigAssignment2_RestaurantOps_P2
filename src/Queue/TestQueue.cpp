#include "Queue/TestQueue.hpp"
#include "Queue/Heap.hpp"
#include "Queue/QueueLRCO.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

void TestQueue::QueueLRCO::test()
{
    ::QueueLRCO<int> test;
    test.enqueue(1);
    test.enqueue(2);
    test.enqueue(3);

    auto iter = test.begin();
    assert(*iter++ == 1);
    assert(*iter++ == 2);
    assert(*iter++ == 3);

    for (auto &elem : test)
        std::cout << elem << ' ';
    std::cout << std::endl;

    test.renew(test.begin());
    iter = test.begin();
    assert(*iter++ == 2);
    assert(*iter++ == 3);
    assert(*iter++ == 1);

    for (auto &elem : test)
        std::cout << elem << ' ';
    std::cout << std::endl;
}

void TestQueue::Heap::testPush0_7()
{
    TestQueue::Heap::WrappredHeapForTest<int, std::less<>> testMinHeap;
    std::vector<int> elemList;

    for (int i = 0; i < 8; i++)
    {
        testMinHeap.push(i);
        elemList.push_back(i);
        testMinHeap.testStructure();
        testMinHeap.testValue(elemList);
    }
}

void TestQueue::Heap::testPop0_7()
{
    TestQueue::Heap::WrappredHeapForTest<int, std::less<>> testMinHeap;
    std::vector<int> elemList;

    for (int i = 0; i < 8; i++)
    {
        testMinHeap.push(i);
        elemList.push_back(i);
    }

    while (!testMinHeap.empty())
    {
        testMinHeap.pop();

        std::swap(elemList.front(), elemList.back());
        elemList.pop_back();

        testMinHeap.testStructure();
        testMinHeap.testValue(elemList);
    }
}

void TestQueue::Heap::testRemove()
{
    TestQueue::Heap::WrappredHeapForTest<int, std::less<>> testMinHeap;
    std::vector<int> elemList;

    for (int i = 0; i < 8; i++)
    {
        testMinHeap.push(i);
        elemList.push_back(i);
    }

    elemList = std::vector<int>({0, 1, 5, 3, 4, 7, 6});
    testMinHeap.remove(2);
    testMinHeap.testValue(elemList);

    elemList = std::vector<int>{0, 1, 5, 3, 4, 6};
    testMinHeap.remove(7);
    testMinHeap.testValue(elemList);

    elemList = std::vector<int>{1, 3, 5, 6, 4};
    testMinHeap.remove(0);
    testMinHeap.testValue(elemList);
}

void TestQueue::Heap::testRandom()
{
    constexpr int ELEM_COUNT = 127;
    constexpr int TEST_COUNT = 1000;

    TestQueue::Heap::WrappredHeapForTest<int, std::less<>> testMinHeap;

    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> elemList;
    for (int i = 0; i < ELEM_COUNT; i++)
    {
        elemList.push_back(i);
    }

    for (int i = 0; i < TEST_COUNT; i++)
    {
        std::shuffle(elemList.begin(), elemList.end(), g);

        for (int elem : elemList)
        {
            testMinHeap.push(elem);
            testMinHeap.testStructure();
        }

        testMinHeap.testValue(elemList);

        std::shuffle(elemList.begin(), elemList.end(), g);

        while (!testMinHeap.empty())
        {
            testMinHeap.pop();
            testMinHeap.testStructure();
        }
    }
}

void TestQueue::Heap::test()
{
    testPush0_7();
    testPop0_7();
    testRemove();
    testRandom();
}

void TestQueue::QueueLFCO::testVectorElemListConstructor()
{
    std::vector<int> valueList({0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 9});

    WrappedQueuLFCOForTest<int, std::less<>> testObject1;
    for (int i = 0; i < 10; i++)
        testObject1.push(i);
    WrappedQueuLFCOForTest<int, std::less<>> testObject2(valueList);

    testObject1.printValue();
    testObject2.printValue();
}

void TestQueue::QueueLFCO::testDistinctElement()
{
    WrappedQueuLFCOForTest<int, std::less<>> testObject;
    for (int i = 0; i < 10; i++)
        testObject.push(i);

    assert(testObject.peek() == 0);

    testObject.pop();
    assert(!testObject.contains(0));

    testObject.remove(5);
    assert(!testObject.contains(5));
}

void TestQueue::QueueLFCO::testAllEquivElement()
{
    WrappedQueuLFCOForTest<int, std::less<>> testObject;
    for (int i = 0; i < 10; i++)
        testObject.push(0);

    assert(testObject.peek() == 0);

    testObject.pop();
    assert(testObject.contains(0));

    testObject.pop();
    assert(testObject.contains(0));
}

void TestQueue::QueueLFCO::testResetCounter()
{
    WrappedQueuLFCOForTest<int, std::less<>> testObject;
    for (int i = 0; i < 10; i++)
        testObject.push(0);

    testObject.printValue();
    testObject.clear();

    for (int i = 0; i < 10; i++)
        testObject.push(0);

    testObject.printValue();
    testObject.clear();
}

void TestQueue::QueueLFCO::testPrint()
{
    WrappedQueuLFCOForTest<int, std::less<>> testObject;
    for (int i = 0; i < 10; i++)
        testObject.push(i);
    testObject.printValue();
    testObject.forEach([](const int &elem) { std::cout << elem << ' '; });
    std::cout << '\n';
}

void TestQueue::QueueLFCO::testRenew()
{
    WrappedQueuLFCOForTest<int, std::less<>> testObject;
    for (int i = 0; i < 10; i++)
        testObject.push(i);

    testObject.renew(0);
    testObject.renew(1);
    testObject.renew(2);
    testObject.renew(3);
    testObject.renew(4);
    testObject.printValue();
}

void TestQueue::QueueLFCO::test()
{
    /* testVectorElemListConstructor(); */
    testDistinctElement();
    testAllEquivElement();
    testResetCounter();
    /* testRenew(); */
    testResetCounter();
}
