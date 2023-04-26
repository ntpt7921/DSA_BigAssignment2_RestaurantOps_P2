#include "AVLTest.hpp"
#include <algorithm>
#include <iostream>
#include <random>

std::vector<int> AVLTest::generateRandomPattern(int count = 1000, int low = 0, int high = 100)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> udist(low, high);

    std::vector<int> result;
    result.reserve(count);
    for (int i = 0; i < count; i++)
        result.push_back(udist(rng));

    return result;
}

std::vector<int> AVLTest::shufflePatternRandomly(const std::vector<int> &pattern)
{
    std::vector<int> result(pattern);
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(result.begin(), result.end(), g);

    return result;
}

bool AVLTest::testInsert_withPattern(const std::vector<int> &pattern)
{
    AVLTree<int, int> testSubject;

    for (int sample : pattern)
    {
        testSubject.insert(sample, sample);
        if (!testSubject.checkTreeStructure())
        {
            std::cerr << "For pattern: ";
            printVector(pattern);
            std::cerr << "Tree is structurally damaged after inserting node " << sample
                      << std::endl;
            std::cerr << "-------" << std::endl;

            return false;
        }
    }

    return true;
}

void AVLTest::testInsert_withRandomPattern(int iter, int patternSize = 20, int low = 0,
                                           int high = 100)
{
    for (int i = 0; i < iter; i++)
    {
        auto pattern = generateRandomPattern(patternSize, low, high);
        std::cerr << "Test " << i + 1 << std::endl;
        testInsert_withPattern(pattern);
    }
}

void AVLTest::testInsert() { testInsert_withRandomPattern(100000, 127, 0, 1000); }

bool AVLTest::testRemove_withPattern(const std::vector<int> &pattern)
{
    AVLTree<int, int> testSubject;

    for (int sample : pattern)
        testSubject.insert(sample, sample);

    auto removePattern = shufflePatternRandomly(pattern);

    for (int removeValue : removePattern)
    {
        testSubject.remove(removeValue);
        if (!testSubject.checkTreeStructure())
        {
            std::cerr << "For pattern: ";
            printVector(pattern);
            std::cerr << "And remove pattern: ";
            printVector(removePattern);

            std::cerr << "Tree is structurally damaged after removing node " << removeValue
                      << std::endl;
            std::cerr << "-------" << std::endl;

            return false;
        }
    }

    return true;
}

void AVLTest::testRemove_withRandomPattern(int iter, int patternSize = 20, int low = 0,
                                           int high = 100)
{
    for (int i = 0; i < iter; i++)
    {
        auto pattern = generateRandomPattern(patternSize, low, high);
        std::cerr << "Test " << i + 1 << std::endl;
        testRemove_withPattern(pattern);
    }
}

void AVLTest::testRemove() { testRemove_withRandomPattern(10000, 127, 0, 1000); }

void AVLTest::shortTest()
{
    AVLTest::testInsert_withRandomPattern(3000, 127, 0, 1000);
    AVLTest::testRemove_withRandomPattern(3000, 127, 0, 1000);
}

void AVLTest::longTest()
{
    AVLTest::testInsert_withRandomPattern(30000, 127, 0, 1000);
    AVLTest::testRemove_withRandomPattern(30000, 127, 0, 1000);
}
