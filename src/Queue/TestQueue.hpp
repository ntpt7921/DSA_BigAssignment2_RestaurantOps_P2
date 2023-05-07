#ifndef QUEUE_TESTQUEUE_HPP
#define QUEUE_TESTQUEUE_HPP

#include "Queue/Heap.hpp"
#include "QueueLFCO.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace TestQueue
{

namespace QueueLRCO
{

void test();

};

namespace Heap
{

template <typename T, typename Compare>
class WrappredHeapForTest : public ::Heap<T, Compare>
{
public:
    void testStructure();
    void testValue(std::vector<T> &sample);
    void printValue();
};

template <typename T, typename Compare>
void TestQueue::Heap::WrappredHeapForTest<T, Compare>::testStructure()
{
    auto elementIsGood = [&](typename std::vector<T>::iterator it) -> bool
    {
        using size_type = typename std::vector<T>::size_type;
        size_type currIndex = it - this->arr.begin();
        size_type leftIndex = 2 * currIndex + 1;
        size_type rightIndex = 2 * currIndex + 2;

        if (leftIndex < this->arr.size())
            if (!this->comp(*it, this->arr[leftIndex]) && !this->equiv(*it, this->arr[leftIndex]))
                return false;

        if (rightIndex < this->arr.size())
            if (!this->comp(*it, this->arr[rightIndex]) && !this->equiv(*it, this->arr[rightIndex]))
                return false;

        return true;
    };

    for (auto it = this->arr.begin(), end = this->arr.end(); it != end; it++)
        assert(elementIsGood(it) && "Heap structure is not maintained");
}

template <typename T, typename Compare>
void TestQueue::Heap::WrappredHeapForTest<T, Compare>::testValue(std::vector<T> &sample)
{
    std::vector<T> currContent(this->arr);  // copy a new vector
    std::sort(currContent.begin(), currContent.end());
    std::sort(sample.begin(), sample.end());

    assert((currContent == sample) && "Value within heap is not the same as provided sample");
}

template <typename T, typename Compare>
void TestQueue::Heap::WrappredHeapForTest<T, Compare>::printValue()
{
    for (const auto &value : this->arr)
        std::cout << value << ' ';
    std::cout << std::endl;
}

void testPush0_7();
void testPop0_7();
void testRemove();
void testRandom();

void test();

};  // namespace Heap

namespace QueueLFCO
{

template <typename T, typename Compare>
class WrappedQueuLFCOForTest : public ::QueueLFCO<T, Compare>
{
public:
    WrappedQueuLFCOForTest() = default;
    WrappedQueuLFCOForTest(const std::vector<T> &elemList);
    void printValue();
};

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &out, const std::pair<T1, T2> &p)
{
    out << '(' << p.first << ',' << p.second << ')';
    return out;
}

template <typename T, typename Compare>
TestQueue::QueueLFCO::WrappedQueuLFCOForTest<T, Compare>::WrappedQueuLFCOForTest(
    const std::vector<T> &elemList)
    : ::QueueLFCO<T, Compare>(elemList)
{
}

template <typename T, typename Compare>
void TestQueue::QueueLFCO::WrappedQueuLFCOForTest<T, Compare>::printValue()
{
    for (const auto &value : this->arr)
        std::cout << value << ' ';
    std::cout << std::endl;
}

void testVectorElemListConstructor();
void testDistinctElement();
void testAllEquivElement();
void testResetCounter();
void testPrint();
void testRenew();
void test();

};  // namespace QueueLFCO

};  // namespace TestQueue

#endif  // !QUEUE_TESTQUEUE_HPP
