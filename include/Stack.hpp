#ifndef STACK_HPP
#define STACK_HPP

template <typename T>
class Stack {
public:
  Stack();
  virtual ~Stack();
  virtual void Push(const T&) = 0;
  virtual void Pop() = 0;
  virtual T Peek() = 0;
  virtual int Size() = 0;
};

#endif