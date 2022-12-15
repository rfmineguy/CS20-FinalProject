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
  int FrontIndex();
  int BackIndex();

private:
  T* buffer;
  int size, maxSize;
  int backIndex, frontIndex;
};

// Template
// =====================================================================================
// Name
//   - Notes
//   - Analysis
//
// =====================================================================================


// Template
// =====================================================================================
// Constructor for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) We're only allocating memory and initializing the member variables
// =====================================================================================
PFX CircularQueue<T>::CircularQueue(int maxSize)
:buffer(new T[maxSize]), size(0), maxSize(maxSize), backIndex(0), frontIndex(0) {}

// =====================================================================================
// Destructor for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) We're only deleting a heap allocated array
// =====================================================================================
PFX CircularQueue<T>::~CircularQueue() {
  delete[] buffer;
  buffer = nullptr;
}

// =====================================================================================
// Clear for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) No loops are required to clear the circular queue
// =====================================================================================
PFX void CircularQueue<T>::Clear() {
  backIndex = 0;
  frontIndex = 0;
  delete[] buffer;
  buffer = nullptr;
  buffer = new T[maxSize];
}

// =====================================================================================
// Enqueue for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) Enqueing to this circular queue is a mathmatical operation (comes out as linear in analysis)
// =====================================================================================
PFX void CircularQueue<T>::Enqueue(const T& elem) {
  if (Size() >= maxSize)
    throw std::string("Enqueue: Full queue");
  buffer[backIndex] = elem;
  backIndex += 1;
  backIndex = backIndex % maxSize;
  size++;
}

// =====================================================================================
// Dequeue for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) Dequeing from this circular queue is a mathmatical operation (comes out as linear in analysis)
// =====================================================================================
PFX void CircularQueue<T>::Dequeue() {
  if (Size() == 0)
    throw std::string("Dequeue: Empty queue");
  frontIndex += 1;
  frontIndex = frontIndex % maxSize;
  size --;
}

// =====================================================================================
// Front for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) Accessing front is a direct memory access
// =====================================================================================
PFX T CircularQueue<T>::Front() const {
  return buffer[frontIndex];
}

// =====================================================================================
// Back for CircularQueue<T>
//   - Notes
//   - Analysis
//     \__ O(1) Accessing back is a direct memory access
// =====================================================================================
PFX T CircularQueue<T>::Back() const {
  return buffer[backIndex];
}

// =====================================================================================
// FrontIndex for CircularQueue<T>
//   - Notes
//     \__ Used for retreiving the actual array index of this queue (not to be used frequently)
//   - Analysis
//     \__ O(1)
// =====================================================================================
PFX int CircularQueue<T>::FrontIndex() {
  return this->frontIndex;
}

// =====================================================================================
// BackIndex for CircularQueue<T>
//   - Notes
//     \__ Used for retreiving the actual array index of this queue (not to be used frequently)
//   - Analysis
//     \__ O(1)
// =====================================================================================
PFX int CircularQueue<T>::BackIndex() {
  return this->backIndex;
}

// =====================================================================================
// Size for CircularQueue<T>
//   - Notes
//     \__ Used for retreiving logical size of the queue
//   - Analysis
//     \__ O(1)
// =====================================================================================
PFX int CircularQueue<T>::Size() const {
  return size;
}

// =====================================================================================
// ostream overload for CircularQueue<T>
//   - Notes
//     \__ Used internally by std::ostream to have more advanced display for CircularQueue
//   - Analysis
//     \__ O(n) Loops over the entire logical array
// =====================================================================================
template <typename C>
std::ostream& operator<<(std::ostream& os, const CircularQueue<C>& circQueue) {
  for (int i = circQueue.frontIndex; i < circQueue.Size() + circQueue.frontIndex; i++) {
    os << circQueue.buffer[i % circQueue.maxSize] << ",";
  }
  return os;
}
#endif