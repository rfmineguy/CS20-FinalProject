#ifndef STACK_HPP
#define STACK_HPP
#define PFX template<typename T>

template <typename T>
class Stack {
public:
  Stack(): m_Size(0) {};
  virtual ~Stack() {};
  virtual void Clear() = 0;
  virtual void Push(const T&) = 0;
  virtual void Pop() = 0;
  virtual T Peek() = 0;
  virtual int Size() const = 0;
  
protected:
  int m_Size;
};

#endif
