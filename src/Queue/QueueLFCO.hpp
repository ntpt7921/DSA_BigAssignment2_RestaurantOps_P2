#ifndef QUEUE_QUEUELFCO_HPP
#define QUEUE_QUEUELFCO_HPP

#include "Queue/Heap.hpp"
#include <cstdint>
#include <iterator>
#include <utility>

using _QueueLFCOInsertOrder = uint_fast32_t;

// NOTE:
// This function object type takes a normal Compare object and add comparing a fallback order value
// in case argument to the normal Compare is equivalent. The fallback order value is stored as a
// pair with first being the order, second being normal value.
template <typename T, typename Compare>
struct CompareWithOrderFallback
{
    const Compare cmp = Compare();  // default init-ed compare function object for type T

    bool operator()(const std::pair<_QueueLFCOInsertOrder, T> &p1,
                    const std::pair<_QueueLFCOInsertOrder, T> &p2) const
    {
        if (cmp(p1.second, p2.second))
            return true;
        else if (cmp(p2.second, p1.second))
            return false;
        else  // p1.second and p2.second is equivalent
            return (p1.first < p2.first);
    }
};

// NOTE:
// Implement the required LFCO queue with insert order used as fallback comparison criteria when
// normal values of type T is equivalent. Each element will carry a unique insert order value,
// ensuring of no element is equivalent to others within the queue
template <typename T, typename Compare>
class QueueLFCO
    : public Heap<std::pair<_QueueLFCOInsertOrder, T>, CompareWithOrderFallback<T, Compare>>
{
protected:
    _QueueLFCOInsertOrder
        insertOrderCount;  // count the number of insert (push) into the queue; this value is reset
                           // when heap is empty, making it harder for this value to overflow

public:
    QueueLFCO();
    ~QueueLFCO();

    bool contains(const T &item) const;

    T &peek();
    const T &peek() const;

    void push(const T &item);
    void pop();
    void remove(const T &item);
    void clear();
};

template <typename T, typename Compare>
QueueLFCO<T, Compare>::QueueLFCO()
    : Heap<std::pair<_QueueLFCOInsertOrder, T>, CompareWithOrderFallback<T, Compare>>(),
      insertOrderCount(0)
{
}

template <typename T, typename Compare>
QueueLFCO<T, Compare>::~QueueLFCO()
{
}

template <typename T, typename Compare>
bool QueueLFCO<T, Compare>::contains(const T &item) const
{
    auto hasElementWithinPair = [&](const std::pair<_QueueLFCOInsertOrder, T> &p) -> bool
    { return (item == p.second); };

    if (this->empty())
        return false;
    else
        return (std::find_if(this->arr.begin(), this->arr.end(), hasElementWithinPair)
                != this->arr.end());
}

template <typename T, typename Compare>
T &QueueLFCO<T, Compare>::peek()
{
    return this->arr.front().second;
}

template <typename T, typename Compare>
const T &QueueLFCO<T, Compare>::peek() const
{
    return this->arr.front().second;
}

template <typename T, typename Compare>
void QueueLFCO<T, Compare>::push(const T &item)
{
    this->arr.push_back(std::pair<_QueueLFCOInsertOrder, T>(this->insertOrderCount++, item));
    this->reheapUp(std::prev(this->arr.end()));
}

template <typename T, typename Compare>
void QueueLFCO<T, Compare>::pop()
{
    Heap<std::pair<_QueueLFCOInsertOrder, T>, CompareWithOrderFallback<T, Compare>>::pop();
    if (this->size() == 0)
        this->insertOrderCount = 0;
}

template <typename T, typename Compare>
void QueueLFCO<T, Compare>::remove(const T &item)
{
    auto hasElementWithinPair = [&](const std::pair<_QueueLFCOInsertOrder, T> &p) -> bool
    { return (item == p.second); };

    auto foundAt = std::find_if(this->arr.begin(), this->arr.end(), hasElementWithinPair);

    if (foundAt != this->arr.end())
    {
        // put last elem at the need-to-be-removed position, then reheapDown
        *foundAt = this->arr.back();
        this->arr.pop_back();
        this->reheapDown(foundAt);
    }

    if (this->size() == 0)
        this->insertOrderCount = 0;
}

template <typename T, typename Compare>
void QueueLFCO<T, Compare>::clear()
{
    Heap<std::pair<_QueueLFCOInsertOrder, T>, CompareWithOrderFallback<T, Compare>>::clear();
    this->insertOrderCount = 0;
}

#endif  // !QUEUE_QUEUELFCO_HPP
