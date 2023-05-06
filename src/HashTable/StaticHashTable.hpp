#ifndef HASHTABLE_HASHTABLE_HPP
#define HASHTABLE_HASHTABLE_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <functional>

template <typename T, std::size_t N>
class StaticHashTable
{
protected:
    using size_type = std::size_t;

    std::array<std::pair<bool, T>, N> arr;
    size_type occupiedCount;

public:
    StaticHashTable();
    ~StaticHashTable();

    size_type size() const;

    bool add(std::size_t key, const T &value);
    bool emptyAt(std::size_t key) const;
    T &operator[](std::size_t key);
    void print(std::function<void(const T &)> printFunction);
};

template <typename T, std::size_t N>
StaticHashTable<T, N>::StaticHashTable() : occupiedCount(0)
{
}

template <typename T, std::size_t N>
StaticHashTable<T, N>::~StaticHashTable()
{
}

template <typename T, std::size_t N>
typename StaticHashTable<T, N>::size_type StaticHashTable<T, N>::size() const
{
    return this->occupiedCount;
}

template <typename T, std::size_t N>
bool StaticHashTable<T, N>::add(std::size_t key, const T &value)
{
    assert(key < N && "Key is not in range");

    if (occupiedCount >= N)
        return false;

    size_t targetIndex = key;
    while (!emptyAt(targetIndex))
    {
        targetIndex++;
        if (targetIndex >= N)
            targetIndex %= N;
    }

    arr[targetIndex].first = true;
    arr[targetIndex].second = value;

    occupiedCount++;
    return true;
}

template <typename T, std::size_t N>
bool StaticHashTable<T, N>::emptyAt(std::size_t key) const
{
    assert(key < N && "Key is not in range");
    return !(arr[key].first);
}

template <typename T, std::size_t N>
T &StaticHashTable<T, N>::operator[](std::size_t key)
{
    assert(key < N && "Key is not in range");
    return arr[key].second;
}

template <typename T, std::size_t N>
void StaticHashTable<T, N>::print(std::function<void(const T &)> printFunction)
{
    for (const auto &elem : this->arr)
        if (elem.first)
            printFunction(elem.second);
}

#endif  // !HASHTABLE_HASHTABLE_HPP
