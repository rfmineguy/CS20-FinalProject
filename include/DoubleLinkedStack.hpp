#ifndef DOUBLE_LINKED_STACK_HPP
#define DOUBLE_LINKED_STACK_HPP
#define PFX template<typename T>

#include "Stack.hpp"
#include <iostream>

/*
 DEFINITION
*/
template <typename T>
class DoubleLinkedStack : public Stack<T> {
public:
  DoubleLinkedStack();
  DoubleLinkedStack(const DoubleLinkedStack&);
  virtual ~DoubleLinkedStack();
  DoubleLinkedStack operator=(const DoubleLinkedStack&);
  void Clear() override;
  void Push(const T&) override;
  void Pop() override;
  T Peek() override;
  int Size() const override;
  
  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const DoubleLinkedStack<C>&);

private:
  void copy(const DoubleLinkedStack&);
  
protected:
  struct Node {
    T value;
    Node* next;
    Node* prev;
    Node(T _value): value(_value), next(nullptr), prev(nullptr) {}
  };
private:
  Node* head;
  Node* tail;
};

// Template
// =====================================================================================
// Name
//   - Notes
//   - Analysis
//
// =====================================================================================

// =====================================================================================
// Constructor for DoubleLinkedStack
//   - Notes
//   - Analysis (1)
// =====================================================================================
PFX DoubleLinkedStack<T>::DoubleLinkedStack()
:head(nullptr), tail(nullptr) { }

// =====================================================================================
// Destructor for DoubleLinkedStack
//   - Notes
//   - Analysis (1)
// =====================================================================================
PFX DoubleLinkedStack<T>::~DoubleLinkedStack() {
  Clear();
}

// =====================================================================================
// Copy Constructor for DoubleLinkedStack
//   - Notes
//   - Analysis O(Clear) + O(copy)
// =====================================================================================
PFX DoubleLinkedStack<T>::DoubleLinkedStack(const DoubleLinkedStack<T>& stack) {
  Clear();
  copy(stack);
}

// =====================================================================================
// Assign operator for DoubleLinkedStack
//   - Notes
//   - Analysis:
//     \__ O(Clear) + O(copy) when not self assign
// =====================================================================================
PFX DoubleLinkedStack<T> DoubleLinkedStack<T>::operator=(const DoubleLinkedStack<T>& stack) {
  if (this != &stack) {
    Clear();
    copy(stack);
  }
}

// =====================================================================================
// Clear function for DoubleLinkedStack
//   - Notes
//   - Analysis (1)
// =====================================================================================
PFX void DoubleLinkedStack<T>::Clear() {
  
}

// =====================================================================================
// copy function for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(1) When the stack is empty
//     \__ O(n) When the incoming stack is not empty
// =====================================================================================
PFX void DoubleLinkedStack<T>::copy(const DoubleLinkedStack& stack) {
  std::cout << "Copying stack" << std::endl;
  if (stack.head == nullptr) {
    return;
  }
  this->head = new Node(stack.head->value);

  Node* n = stack.head;
  Node* this_n = this->head;
  while (n->next != nullptr) {
    Node* newNode = new Node(n->next->value);
    this_n->next = newNode;
    this_n = this_n->next;
    n = n->next;
  }
  this->m_Size = stack.m_Size;
  std::cout << "Copied stack" << std::endl;
}

// =====================================================================================
// Push function for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(1) When the stack is empty and not empty
// =====================================================================================
PFX void DoubleLinkedStack<T>::Push(const T& value) {
  Node* n = new Node(value);
  if (head == nullptr) {
    this->head = n;
    this->tail = n;
    this->tail->next = nullptr;
    this->head->prev = nullptr;
  }
  else {
    this->head->prev = n;
    n->next = this->head;
    this->head = n;
    this->head->prev = nullptr;
  }
  this->m_Size ++;
}

// =====================================================================================
// Pop function for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(1) When the stack is empty and not empty
// =====================================================================================
PFX void DoubleLinkedStack<T>::Pop() {
  if (head == nullptr) {
    throw std::string("Empty");
  }
  if (head != tail) {
    Node* n = head;
    head = head->next;
    head->prev = nullptr;
    delete n;
    n = nullptr;
  }
  else {
    delete head;
    head = nullptr;
  }
  
  this->m_Size --;
}

// =====================================================================================
// Peek function for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(1) When the stack is empty and not empty
// =====================================================================================
PFX T DoubleLinkedStack<T>::Peek() {
  if (Size() == 0) {
    throw std::string("Peek fail. No elements in stack.");
  }
  return tail->value;
}

// =====================================================================================
// Size function for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(1)
// =====================================================================================
PFX int DoubleLinkedStack<T>::Size() const {
  return this->m_Size;
}

// =====================================================================================
// ostream operator for DoubleLinkedStack
//   - Notes
//   - Analysis
//     \__ O(n) in all cases
// =====================================================================================
template <typename C>
std::ostream& operator<<(std::ostream& os, const DoubleLinkedStack<C>& stack) {
  typename DoubleLinkedStack<C>::Node* n = stack.head;
  while (n != nullptr) {
    os << n->value << " --> ";
    if (n->next == nullptr) {
      os << "nullptr";
    }
    n = n->next;
  }
  os << std::endl;
  return os;
}
#endif