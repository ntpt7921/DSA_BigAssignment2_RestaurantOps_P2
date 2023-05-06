#include "HuffmanCoding/TestHuffmanCoding.hpp"
#include "HuffmanCoding/Heap.hpp"
#include "HuffmanCoding/HuffmanTree.hpp"
#include "HuffmanCoding/HuffmanTreeNode.hpp"
#include "HuffmanCoding/HuffmanTreeWrapper.hpp"
#include <cassert>
#include <cwchar>
#include <iostream>
#include <utility>
#include <vector>

using namespace details;

struct IntSmaller
{
    static bool prior(int i1, int i2) { return i1 < i2; }
};

void printArr(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        std::cerr << arr[i] << ' ';
    std::cerr << '\n';
}

void TestHuffmanCoding::Heap::testInsert()
{
    int arr[7] = {6, 5, 4, 3, 2, 1, 0};
    ::heap<int, IntSmaller> testHeap(arr, 7, 7);
    assert(testHeap.removefirst() == 0);
    assert(testHeap.removefirst() == 1);
    assert(testHeap.removefirst() == 2);
    assert(testHeap.removefirst() == 3);
    assert(testHeap.removefirst() == 4);
    assert(testHeap.removefirst() == 5);
    assert(testHeap.removefirst() == 6);
}

void TestHuffmanCoding::Heap::test() { testInsert(); }

void TestHuffmanCoding::HuffTree::testMemoryLeakageOneLeafNode()
{
    ::HuffTree<char> *testTree = new ::HuffTree<char>('a', 1);
    delete testTree;
}

void TestHuffmanCoding::HuffTree::testMemoryLeakage()
{
    constexpr int NUM_TREE = 15;
    std::vector<::HuffTree<char> *> newTreeArr(NUM_TREE);

    for (int i = 0; i < NUM_TREE; i++)
        newTreeArr[i] = new ::HuffTree<char>('a' + i, 1 + i);

    auto resultTree = buildHuff(newTreeArr);

    assert(resultTree->weight() == (NUM_TREE) * (NUM_TREE + 1) / 2);
    delete resultTree;
}

void TestHuffmanCoding::HuffTree::test()
{
    testMemoryLeakageOneLeafNode();
    testMemoryLeakage();
}

void TestHuffmanCoding::HuffmanTreeWrapper::testSamplePattern()
{
    std::vector<std::pair<char, std::size_t>> elemLists = {
        {'z', 2}, {'k', 7}, {'m', 24}, {'c', 32}, {'u', 37}, {'d', 42}, {'l', 42}, {'e', 120},
    };
    ::HuffmanTreeWrapper<char> testObject(elemLists);

    // example from within the book
    std::string deed = "deed";
    std::string encodedDeed;
    std::string muck = "muck";
    std::string encodedMuck;

    for (char c : deed)
        encodedDeed += testObject.getCoding(c);
    for (char c : muck)
        encodedMuck += testObject.getCoding(c);

    assert(encodedDeed == "10100101");
    assert(encodedMuck == "111111001110111101");
}

void TestHuffmanCoding::HuffmanTreeWrapper::testSamplePatternProvided()
{
    std::vector<std::pair<char, std::size_t>> elemLists = {
        {'J', 1}, {'o', 1}, {'n', 1}, {'u', 1}, {'g', 1}, {'i', 2}, {'s', 2},
        {'d', 2}, {'b', 3}, {'a', 3}, {'j', 3}, {'h', 4}, {'f', 4}};
    ::HuffmanTreeWrapper<char> testObject(elemLists);

    // example from within the book
    std::string testString = "Johnuigfifbahjasbdfhjbasdhjf";
    std::string encodedTestString;

    assert(testObject.getCoding('f') == "100");
    assert(testObject.getCoding('j') == "010");
    assert(testObject.getCoding('h') == "101");
    assert(testObject.getCoding('d') == "0111");
    assert(testObject.getCoding('s') == "1101");
}

void TestHuffmanCoding::HuffmanTreeWrapper::testCalculateMultipleTree()
{
    std::vector<std::pair<char, std::size_t>> elemLists = {
        {'z', 2}, {'k', 7}, {'m', 24}, {'c', 32}, {'u', 37}, {'d', 42}, {'l', 42}, {'e', 120},
    };
    ::HuffmanTreeWrapper<char> testObject(elemLists);

    testObject.calculateCoding(elemLists);
}

void TestHuffmanCoding::HuffmanTreeWrapper::test()
{
    testSamplePattern();
    testSamplePatternProvided();
    testCalculateMultipleTree();
}
