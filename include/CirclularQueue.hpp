#ifndef CIRCLUAR_QUEUE_HPP
#define CIRCLUAR_QUEUE_HPP
#define PFX template<typename T>

#include <string>
#include <iostream>
#include "Queue.hpp"

template <typename T>
class CircularQueue : public Queue<T> {
public:
  CircularQueue(int=10);
  ~CircularQueue();
  void Clear();
  void Enqueue(const T&);
  void Dequeue();
  T Back() const;
  T Front() const;
  int Size() const;

  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const CircularQueue<C>&);

public:
  int InternalGetFrontIndex();
  int InternalGetBackIndex();

private:
  T* buffer;
  int size, maxSize;
  int backIndex, frontIndex;
};

PFX CircularQueue<T>::CircularQueue(int maxSize)
:buffer(new T[maxSize]), size(0), maxSize(maxSize), backIndex(0), frontIndex(0) {}

PFX CircularQueue<T>::~CircularQueue() {
  delete[] buffer;
  buffer = nullptr;
}

PFX void CircularQueue<T>::Clear() {
  backIndex = 0;
  frontIndex = 0;
  delete[] buffer;
  buffer = nullptr;
  buffer = new T[maxSize];
}

PFX void CircularQueue<T>::Enqueue(const T& elem) {
  if (Size() >= maxSize)
    throw std::string("Enqueue: Full queue");
  buffer[backIndex] = elem;
  backIndex += 1;
  backIndex = backIndex % maxSize;
  size++;
}

PFX void CircularQueue<T>::Dequeue() {
  if (Size() == 0)
    throw std::string("Dequeue: Empty queue");
  frontIndex += 1;
  frontIndex = frontIndex % maxSize;
  size --;
}

PFX T CircularQueue<T>::Front() const {
  return buffer[frontIndex];
}

PFX T CircularQueue<T>::Back() const {
  return buffer[backIndex];
}

PFX int CircularQueue<T>::InternalGetFrontIndex() {
  return this->frontIndex;
}

PFX int CircularQueue<T>::InternalGetBackIndex() {
  return this->backIndex;
}

PFX int CircularQueue<T>::Size() const {
  return size;
}

template <typename C>
std::ostream& operator<<(std::ostream& os, const CircularQueue<C>& circQueue) {
  for (int i = circQueue.frontIndex; i < circQueue.Size() + circQueue.frontIndex; i++) {
    os << circQueue.buffer[i % circQueue.maxSize] << ",";
  }
  return os;
}
#endif