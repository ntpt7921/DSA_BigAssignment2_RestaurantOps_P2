#ifndef QUEUE_QUEUELRCO_HPP
#define QUEUE_QUEUELRCO_HPP

#include "Queue/QueueFIFO.hpp"
#include <iterator>

template <typename T>
class QueueLRCO : public QueueFIFO<T>
{
public:
    // move the element to the end of the list
    typename QueueFIFO<T>::iterator renew(typename QueueFIFO<T>::iterator iter);
    typename QueueFIFO<T>::iterator renew(typename QueueFIFO<T>::const_iterator citer);
};

template <typename T>
typename QueueFIFO<T>::iterator QueueLRCO<T>::renew(typename QueueFIFO<T>::iterator iter)
{
    if (std::next(iter) == this->end())
        return this->end();

    this->push_back(*iter);
    this->erase(iter);
    return std::prev(this->end());
}

template <typename T>
typename QueueFIFO<T>::iterator QueueLRCO<T>::renew(typename QueueFIFO<T>::const_iterator citer)
{
    if (std::next(citer) == this->cend())
        return this->end();

    this->push_back(*citer);
    this->erase(citer);
    return std::prev(this->end());
}

#endif  // !QUEUE_QUEUELRCO_HPP
