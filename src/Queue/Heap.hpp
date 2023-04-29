#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T, typename Compare>
class Heap
{
protected:
    std::vector<T> arr;
    const Compare comp;

    bool equiv(const T &a, const T &b) { return !comp(a, b) && !comp(b, a); }

public:
    Heap();
    ~Heap();

    typename std::vector<T>::size_type size() const;
    bool empty() const;
    bool contains(const T &item) const;

    T &peek();
    const T &peek() const;

    void push(const T &item);
    void pop();
    void remove(const T &item);
    void clear();

    void reheapUp(typename std::vector<T>::iterator at);
    void reheapDown(typename std::vector<T>::iterator at);
};

template <typename T, typename Compare>
Heap<T, Compare>::Heap() : comp()
{
}

template <typename T, typename Compare>
Heap<T, Compare>::~Heap()
{
}

template <typename T, typename Compare>
typename std::vector<T>::size_type Heap<T, Compare>::size() const
{
    return arr.size();
}

template <typename T, typename Compare>
bool Heap<T, Compare>::empty() const
{
    return arr.empty();
}

template <typename T, typename Compare>
bool Heap<T, Compare>::contains(const T &item) const
{
    if (this->empty())
        return false;
    else
        return (std::find(arr.begin(), arr.end(), item) != arr.end());
}

template <typename T, typename Compare>
T &Heap<T, Compare>::peek()
{
    return arr.front();
}

template <typename T, typename Compare>
const T &Heap<T, Compare>::peek() const
{
    return arr.front();
}

template <typename T, typename Compare>
void Heap<T, Compare>::push(const T &item)
{
    arr.push_back(item);
    reheapUp(std::prev(arr.end()));
}

template <typename T, typename Compare>
void Heap<T, Compare>::pop()
{
    arr.front() = arr.back();
    arr.pop_back();
    reheapDown(arr.begin());
}

template <typename T, typename Compare>
void Heap<T, Compare>::remove(const T &item)
{
    auto foundAt = std::find(arr.begin(), arr.end(), item);

    if (foundAt != arr.end())
    {
        // put last elem at the need-to-be-removed position, then reheapDown
        *foundAt = arr.back();
        arr.pop_back();
        reheapDown(foundAt);
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::clear()
{
    arr.clear();
}

template <typename T, typename Compare>
void Heap<T, Compare>::reheapUp(typename std::vector<T>::iterator at)
{
    auto currIndex = at - arr.begin();
    auto parentIndex = (currIndex - 1) / 2;
    auto temp = *at;

    while (currIndex != 0)
    {
        parentIndex = (currIndex - 1) / 2;
        if (comp(temp, arr[parentIndex]))
            arr[currIndex] = arr[parentIndex];
        else
            break;

        currIndex = parentIndex;
    }
    arr[currIndex] = temp;
}

template <typename T, typename Compare>
void Heap<T, Compare>::reheapDown(typename std::vector<T>::iterator at)
{
    auto currIndex = static_cast<typename std::vector<T>::size_type>(at - arr.begin());
    auto size = arr.size();
    auto temp = *at;
    auto minIndex = currIndex;

    while (currIndex < size)
    {
        auto leftIndex = 2 * currIndex + 1;
        auto rightIndex = 2 * currIndex + 2;

        auto isInRange = [&](auto index) { return index < arr.size(); };

        if (!isInRange(leftIndex))
            break;

        minIndex = leftIndex;

        if (isInRange(rightIndex))
            minIndex = (comp(arr[leftIndex], arr[rightIndex])) ? leftIndex : rightIndex;

        if (comp(arr[minIndex], temp))
            arr[currIndex] = arr[minIndex];
        else
            break;

        currIndex = minIndex;
    }

    arr[currIndex] = temp;
}

#endif
