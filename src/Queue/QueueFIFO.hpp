#ifndef QUEUE_QUEUEFIFO_HPP
#define QUEUE_QUEUEFIFO_HPP

#include <list>

template <typename T>
class QueueFIFO : public std::list<T>
{
public:
    void enqueue(const T &value);
    void dequeue();
};

template <typename T>
void QueueFIFO<T>::enqueue(const T &value)
{
    this->push_back(value);
}

template <typename T>
void QueueFIFO<T>::dequeue()
{
    this->pop_front();
}

#endif  // !QUEUE_QUEUEFIFO_HPP
