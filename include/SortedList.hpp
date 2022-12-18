#ifndef SORTED_LIST_HPP
#define SORTED_LIST_HPP

#ifndef PFX
#define PFX template <typename T>
#endif

#include "List.hpp"
#include <functional>
#include <string>
#include <sstream>
#include <iostream>

// Note this class won't work properly if your ADT doesn't have a operator<< or an operator<
template <typename T>
class SortedList : public List<T> {
protected:
  struct Node {
    // Data    T val;
    T val;
    Node *prev, *next;
    
    // Record constructors
    Node(): val(T()), prev(nullptr), next(nullptr) {}
    Node(T _val): val(_val), prev(nullptr), next(nullptr) {}
  };

public:
  SortedList(std::function<bool(T, T)>, T, T);
  ~SortedList();
  void Clear();
  T At(int index) const;
  void Insert(int index, T elem);
  int Insert(T elem);
  void Replace(int index, T elem);
  void Remove(int elem);
  int Length() const;

  template <typename C>
  friend std::ostream& operator<<(std::ostream&, const SortedList<C>&);

private:
  void clearRec(Node*);
  T atRec(Node*, int, int) const;
  Node* atRecNode(Node*, int, int) const;
  int insertNormalRec(Node*&, T elem, int current);

private:
  std::function<bool(T, T)> sortRule;
  Node *header, *trailer, *middle;
  int length;
};

//================================================================
//   NOTE: NOTE: NOTE: NOTE:
//   DO THIS RECURSIVELY
//================================================================

// =====================================================================================
// Constructor for SortedList<T>
//   - Notes
//   - Analysis
//     \__ O(1) We're only allocating memory and initializing the member variables, and setting up
//                 weird things relating to the sort rule
// =====================================================================================
PFX SortedList<T>::SortedList(std::function<bool(T, T)> _sortRule, T lowest, T highest)
:sortRule(_sortRule), header(new Node()), trailer(new Node()), length(0) {
  header->next = trailer;
  middle = header;
  trailer->prev = header;
  //sortRule => a <= b
  if (sortRule(lowest, highest)) {
    header->val = lowest;
    trailer->val = highest;
  }
  else {
    header->val = highest;
    trailer->val = lowest;
  }
}

// =====================================================================================
// Destructor for SortedList<T>
//   - Notes
//   - Analysis
//     \__ O(Clear) Since Clear is the only thing in this the analysis for it is the same
// =====================================================================================
PFX SortedList<T>::~SortedList() {
  Clear();
}

// =====================================================================================
// Clear function for SortedList<T>
//   - Notes
//   - Analysis
//     \__ O(clearRec) Since clearRec is the only thing in this the analysis for it is the same
// =====================================================================================
PFX void SortedList<T>::Clear() {
  clearRec(header);
  length = 0;
}

// =====================================================================================
// clearRec function for SortedList<T>
//   - Notes
//     \__ Recursive
//   - Analysis
//     \__ O(n) Has to recursively visit each node in the internal linked list
// =====================================================================================
PFX void SortedList<T>::clearRec(Node* n) {
  if (n == nullptr) {
    return;
  }
  clearRec(n->next);
  delete n;
}

// =====================================================================================
// At(int) function for SortedList<T>
//   - Notes
//     \__ Non recursive interface for the recursive version
//   - Analysis
//     \__ O(atRec)
// =====================================================================================
PFX T SortedList<T>::At(int index) const {
  return atRec(header->next, index, 0);
}

// =====================================================================================
// atRec(Node*, int, int) function for SortedList<T>
//   - Notes
//     \__ Recursively find the target node
//   - Analysis
//     \__ O(n) when the value is not present
//     \__ O(n) as well when the value is found (though each node may not need to be visited) 
// =====================================================================================
PFX T SortedList<T>::atRec(Node* n, int targetIndex, int currentIndex) const {
  if (n == nullptr) {
    throw std::string("Value not in list");
  }
  if (currentIndex == targetIndex) {
    return n->val;
  }
  return atRec(n->next, targetIndex, currentIndex + 1);
}

// =====================================================================================
// atRecNode(Node*, int, int) function for SortedList<T>
//   - Notes
//     \__ Recursively find the target node
//   - Analysis
//     \__ O(n) when the value is not present
//     \__ O(n) as well when the value is found (though each node may not need to be visited) 
// =====================================================================================
PFX typename SortedList<T>::Node* SortedList<T>::atRecNode(Node* n, int targetIndex, int currentIndex) const {
  if (n == nullptr) {
    throw std::string("Value not in list");
  }
  if (currentIndex == targetIndex) {
    return n;
  }
  return atRecNode(n->next, targetIndex, currentIndex + 1);
}


// =====================================================================================
// Insert(int index, T elem) function for SortedList<T>
//   - Notes
//     \__ Insert in this form is unsupported for a sorted list
// =====================================================================================
PFX void SortedList<T>::Insert(int index, T elem) {
  std::stringstream ss;
  ss << "Insert(" << index << ", " << elem << ") is undefined for sorted lists";
  throw ss.str();
}

// =====================================================================================
// Insert(T elem) function for SortedList<T>
//   - Notes
//     \__ Insert the value 'elem' sorted
//   - Analysis
//     \__ O(insertNormalRec)
// =====================================================================================
PFX int SortedList<T>::Insert(T elem) {
  try {
    return insertNormalRec(header, elem, 0);
  } catch (std::string& e) {
    std::cerr << e << std::endl;
  }
  return -1;
}

// =====================================================================================
// insertNormalRec(Node*&, T elem, int current) function for SortedList<T>
//   - Notes
//     \__ Insert the value 'elem' sorted, and return where it was inserted
//   - Analysis
//     \__ O(1) When the header is the trailer (MAY BE REDUNDANT)
//     \__ O(n) In all other cases
// =====================================================================================
PFX int SortedList<T>::insertNormalRec(Node*& n, T elem, int current) {
  if (n == trailer) {
    throw std::string("Failed to insert");
  }
  else if (sortRule(n->val, elem) && !sortRule(n->next->val, elem)) {
    // insert elem after
    Node *newNode = new Node(elem);
    Node *next = n->next;
    n->next = newNode;
    newNode->prev = n;
    newNode->next = next;
    next->prev = newNode;
    length++;
    return current;
  }
  else {
    return insertNormalRec(n->next, elem, current + 1);
  }
}

// =====================================================================================
// Replace(int index, T elem) function for SortedList<T>
//   - Notes
//     \__ Replace in this form is unsupported for a sorted list
// =====================================================================================
PFX void SortedList<T>::Replace(int index, T elem) {
  std::stringstream ss;
  ss << "Replace(" << index << ", " << elem << ") is undefined for sorted lists";
  throw ss.str();
}

// =====================================================================================
// Remove(int index) function for SortedList<T>
//   - Notes
//     \__ Replace in this form is unsupported for a sorted list
// =====================================================================================
PFX void SortedList<T>::Remove(int index) {
  std::stringstream ss;
  ss << "Remove(" << index << ") is undefined for sorted lists";
  throw ss.str();
}

// =====================================================================================
// Length(void) function for SortedList<T>
//   - Notes
//     \__ Returns the logical length of the sorted list
//   - Analysis
//     \__ O(1)
// =====================================================================================
PFX int SortedList<T>::Length() const {
  return this->length;
}

// =====================================================================================
// ostream operator for SortedList<T>
//   - Notes
//     \__ Provides a way for std::ostream to display this class to standard out
//   - Analysis
//     \__ O(n) Loops over the whole list
// =====================================================================================
template <typename C>
std::ostream& operator<<(std::ostream& os, const SortedList<C>& sorted_list) {
  typename SortedList<C>::Node* curr = sorted_list.header->next;
  //std::cout << "Printing SortedList :" << curr << std::endl;
  os << "header -> ";
  while (curr != nullptr && curr != sorted_list.trailer) {
    os << curr->val;
    if (curr->next != sorted_list.trailer) {
      os << " <-> ";
    }
    curr = curr->next;
  }
  os << " <- trailer";
  os << std::endl;
  return os;
}
#endif