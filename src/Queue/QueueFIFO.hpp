#ifndef QUEUE_QUEUEFIFO_HPP
#define QUEUE_QUEUEFIFO_HPP

#include <iterator>
#include <list>

template <typename T>
class QueueFIFO : public std::list<T>
{
public:
    typename std::list<T>::iterator enqueue(const T &value);
    void dequeue();
};

template <typename T>
typename std::list<T>::iterator QueueFIFO<T>::enqueue(const T &value)
{
    this->push_back(value);
    return std::prev(this->end());
}

template <typename T>
void QueueFIFO<T>::dequeue()
{
    this->pop_front();
}

#endif  // !QUEUE_QUEUEFIFO_HPP
