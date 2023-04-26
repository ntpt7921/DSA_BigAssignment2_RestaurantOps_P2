#ifndef QUEUE_QUEUELRCO_HPP
#define QUEUE_QUEUELRCO_HPP

#include "Queue/QueueFIFO.hpp"
#include <iterator>

template <typename T>
class QueueLRCO : public QueueFIFO<T>
{
public:
    // move the element to the end of the list
    void renew(typename QueueFIFO<T>::iterator iter);
    void renew(typename QueueFIFO<T>::const_iterator citer);
};

template <typename T>
void QueueLRCO<T>::renew(typename QueueFIFO<T>::iterator iter)
{
    auto nextIter = std::next(iter);
    if (nextIter == this->end())
        return;

    this->push_back(*iter);
    this->erase(iter);
}

template <typename T>
void QueueLRCO<T>::renew(typename QueueFIFO<T>::const_iterator citer)
{
    auto nextCiter = std::next(citer);
    if (nextCiter == this->cend())
        return;

    this->push_back(*citer);
    this->erase(citer);
}

#endif  // !QUEUE_QUEUELRCO_HPP
