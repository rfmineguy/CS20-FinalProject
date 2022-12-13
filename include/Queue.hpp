#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename T>
class Queue {
public:
  Queue() {}
  virtual ~Queue() {}
  virtual void Clear() = 0;
  virtual void Enqueue(const T& elem) = 0;
  virtual void Dequeue() = 0;
  virtual T Front() const = 0;
  virtual T Back() const = 0;
  virtual int Size() const = 0;
};

#endif