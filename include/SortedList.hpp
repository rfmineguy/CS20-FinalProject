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
  SortedList(std::function<bool(T, T)>);
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
  int insertRecLeft(Node*&, T elem, int current);
  int insertRecRight(Node*&, T elem, int current);

private:
  std::function<bool(T, T)> sortRule;
  Node *header, *trailer, *middle;
  int length;
};

//================================================================
//   NOTE: NOTE: NOTE: NOTE:
//   DO THIS RECURSIVELY
//================================================================

//================================================================
//   Setup the initial memory for the header and trailer nodes
//   Additionally a sort rule lambda is required to do the sort
//================================================================
PFX SortedList<T>::SortedList(std::function<bool(T, T)> _sortRule)
:sortRule(_sortRule), header(new Node()), trailer(new Node()), length(0) {
  header->next = trailer;
  middle = header;
  trailer->prev = header;
  //sortRule => a <= b
  header->val = INT_MAX * ((sortRule(1, 0) * 2) - 1); //[-INT_MAX, INT_MAX]
  trailer->val = header->val * -1;
  
  std::cout << "Header=" << header->val << std::endl;
}

//================================================================
//   Clear memory used by the internal linked list
// ================================================================
PFX SortedList<T>::~SortedList() {
  Clear();
}

PFX void SortedList<T>::Clear() {
  clearRec(header);
}

PFX void SortedList<T>::clearRec(Node* n) {
  if (n == nullptr) {
    return;
  }
  clearRec(n->next);
  delete n;
  // while (n != nullptr) {
  //   Node* next = n->next;
  //   delete n;
  //   n = nullptr;
  //   n = next;
  // }
}

// ================================================================
//    Supply a non recursive interface for the recursive .At(int) function
// ================================================================
PFX T SortedList<T>::At(int index) const {
  return atRec(header->next, index, 0);
}

// ================================================================
//    Recursive function to find the value at the particular index
// ================================================================
PFX T SortedList<T>::atRec(Node* n, int targetIndex, int currentIndex) const {
  if (n == nullptr) {
    throw std::string("Value not in list");
  }
  if (currentIndex == targetIndex) {
    return n->val;
  }
  return atRec(n->next, targetIndex, currentIndex + 1);
}

PFX typename SortedList<T>::Node* SortedList<T>::atRecNode(Node* n, int targetIndex, int currentIndex) const {
  if (n == nullptr) {
    throw std::string("Value not in list");
  }
  if (currentIndex == targetIndex) {
    return n;
  }
  return atRecNode(n->next, targetIndex, currentIndex + 1);
}

// ================================================================
//    This function is just to make the normal insert function of List<T> not accessable
// ================================================================
PFX void SortedList<T>::Insert(int index, T elem) {
  std::stringstream ss;
  ss << "Insert(" << index << ", " << elem << ") is undefined for sorted lists";
  throw ss.str();
}

// ================================================================
//    Non recursive function to insert the value specified, its position depends on the sorting condition specified
//      Public interface
//      Returns the index where it resides
//    Analysis:
//      
// ================================================================
// header <-> trailer
// insert(4)
//   header <-> 4 <-> trailer
// insert(5)
//   header <-> 4 <-> 5 <-> trailer
// insert(1)
//   header <-> 1 <-> 4 <-> 5 <-> trailer
PFX int SortedList<T>::Insert(T elem) {
  // if (Length() == 0) {
  //       
  // }
  // if (header->next->val > elem) {
  //   // insert at head
  //   Node* n = new Node(elem);
  //   Node* currNext = header->next;
  //   header->next = n;
  //   n->prev = header;
  //   n->next = currNext;
  //   currNext->prev = n;
  //   return 1;
  // }
  
  try {
    return insertNormalRec(header, elem, 0);
  } catch (std::string& e) {
    std::cerr << e << std::endl;
  }
  return -1;
}

PFX int SortedList<T>::insertNormalRec(Node*& n, T elem, int current) {
  if (n == trailer) {
    throw std::string("Failed to insert");
  }
  // else if (n->val < elem) {
  else if (sortRule(n->val, elem) && !sortRule(n->next->val, elem)) {
    // insert elem after
    Node *newNode = new Node(elem);
    Node *next = n->next;
    n->next = newNode;
    newNode->prev = n;
    newNode->next = next;
    next->prev = newNode;
    return current;
  }
  else {
    return insertNormalRec(n->next, elem, current + 1);
  }
}

// ================================================================
//    Recursive function to find the value at the particular index
//      Private interface
//      Depending on whether the element should go on the right or left 'n' will start at a different place
//    NOTE: 
//      Figure out how to incorporate the sort function into this logically
//    Analysis:
//      
// ================================================================
// UNTESTED:
PFX int SortedList<T>::insertRecLeft(Node*& n, T elem, int current) {
  if (n->val < elem && n->next->val >= elem) {
    std::cout << n->val << ", " << n->next->val << std::endl;
    // insert
    Node* new_node = new Node(elem);
    Node* next = n->next;
    n->next = new_node;
    new_node->next = next;
    new_node->prev = n;
    next->prev = new_node;
    length++;
    return current;
  }
  else {
    return insertRecLeft(n->next, elem, current + 1);
  }
  // recursively insert 'elem' at position 'target'
}

// UNTESTED:
PFX int SortedList<T>::insertRecRight(Node*& n, T elem, int current) {
  if (elem > n->prev->val && elem < n->val) {
    std::cout << n->prev->val << ", " << n->val << std::endl;
    Node* newNode = new Node(elem);
    Node* prev = n->prev;
    
    n->prev = newNode;
    newNode->prev = prev;
    newNode->next = n->next;
    prev->next = newNode;
    length++;
    return current;
    //insert
  }
  else {
    return insertRecRight(n->prev, elem, current);
  }
  return 0;
}

// ================================================================
//    Replace is not a supported feature of a SortedList
// ================================================================
PFX void SortedList<T>::Replace(int index, T elem) {
  std::stringstream ss;
  ss << "Replace(" << index << ", " << elem << ") is undefined for sorted lists";
  throw ss.str();
}

// ================================================================
//    Remove is not a supported feature of a SortedList
// ================================================================
PFX void SortedList<T>::Remove(int index) {
  std::stringstream ss;
  ss << "Remove(" << index << ") is undefined for sorted lists";
  throw ss.str();
}

// ================================================================
//    Retrieve the total number of elements in this sorted list
// ================================================================
PFX int SortedList<T>::Length() const {
  return this->length;
}

template <typename C>
std::ostream& operator<<(std::ostream& os, const SortedList<C>& sorted_list) {
  typename SortedList<C>::Node* curr = sorted_list.header->next;
  //std::cout << "Printing SortedList :" << curr << std::endl;
  os << sorted_list.header->val << " -> ";
  while (curr != nullptr && curr != sorted_list.trailer) {
    os << curr->val << " <-> ";
    curr = curr->next;
  }
  os << sorted_list.trailer->val << " <- ";
  os << std::endl;
  return os;
}
#endif
