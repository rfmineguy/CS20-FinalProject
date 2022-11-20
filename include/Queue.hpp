#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename T>
class Queue {
public:
  Queue();
  virtual ~Queue();
  virtual void Enqueue(const T& elem) = 0;
  virtual T Dequeue() = 0;
  virtual int Size() = 0;
};

#endif