#ifndef TESTFUNCTION_HPP
#define TESTFUNCTION_HPP

#include "AVLTree.hpp"
#include <vector>

namespace AVLTest
{

template <typename T>
void printVector(const std::vector<T> &vec)
{
    for (size_t i = 0; i < vec.size() - 1; i++)
        std::cerr << vec[i] << ", ";
    if (vec.size() > 0)
        std::cerr << vec.back() << std::endl;
}

std::vector<int> generateRandomPattern(int count, int low, int high);
std::vector<int> shufflePatternRandomly(const std::vector<int> &pattern);

bool testInsert_withPattern(const std::vector<int> &pattern);
void testInsert_withRandomPattern(int iter, int patternSize, int low, int high);
void testInsert();

bool testRemove_withPattern(const std::vector<int> &pattern);
void testRemove_withRandomPattern(int iter, int patternSize, int low, int high);
void testRemove();

void shortTest();
void longTest();
void sizeTest();
void printTest();

};  // namespace AVLTest

#endif
