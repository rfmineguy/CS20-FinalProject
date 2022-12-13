#ifndef CLOSED_HASHTABLE_HPP
#define CLOSED_HASHTABLE_HPP
#define HT_PFX template<typename Key, typename Val>
#include "HashTable.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

template <typename Key, typename Val>
class ClosedHashTable : public HashTable<Key, Val> {
public:
  ClosedHashTable(Key, Key);
  ~ClosedHashTable();
  int Hash(const Key&) const;
  int Probe(int) const;
  Val Find(const Key&) const;
  void Insert(const Key&, const Val&);
  void Remove(const Key&);
  int Size();
  
  template <typename CKey, typename CVal>
  friend std::ostream& operator<<(std::ostream&, const ClosedHashTable<CKey, CVal>&);
 
private:
  int M;
  int size;
  const Key TOMBSTONE;
  const Key EMPTY;
  typename HashTable<Key, Val>::Record* buffer;
};

// =====================================================================================
//  Implementation details for ClosedHashTable<T> below
// =====================================================================================

// =====================================================================================
// Constructor for ClosedHashTable<T>
//   - Notes
//     \__ M is required to be a power of 2 for this class (M = 2^X)
//   - O(M)
//     \__ initialization of buffer's keys to the EMPTY key
// =====================================================================================
HT_PFX ClosedHashTable<Key, Val>::ClosedHashTable(Key tombstone, Key empty)
:M(8), size(0), TOMBSTONE(tombstone), EMPTY(empty), buffer(new typename HashTable<Key, Val>::Record[M]) {
  for (int i = 0; i < M; i++) {
    buffer[i].key = EMPTY;
  }
}

// =====================================================================================
// Destructor for ClosedHashTable<T>
//   - O(1)
//     \__ simply has to delete the buffer allocation and reset size to 0
// =====================================================================================
HT_PFX ClosedHashTable<Key, Val>::~ClosedHashTable() {
  delete[] buffer;
  buffer = nullptr;
  size = 0;
}

// =====================================================================================
// Hash function for ClosedHashTable<T>
//   - Notes
//     \__ I figured writing a hash function that would break for certain data types was
//            not a good idea, so I instead use the actual byte values to do the calculations
//            instead. The only downside to this is that what was O(1) is now O(n)
//     \__ The only slight oversights might be overflows and/or mod bias
//   - O(n)
//     \__ Required to loop over bytes
// =====================================================================================
HT_PFX int ClosedHashTable<Key, Val>::Hash(const Key& k) const {
  // read each byte of key (regardless of actual underlying type)
  const char* vPtr = (const char*)&k;
  int byteSum = 0;
  for (size_t i = 0; i < sizeof(k); i++) {
    byteSum += *(vPtr + i);
  }
  return byteSum % M;
}

// =====================================================================================
// Probe function for ClosedHashTable<T>
//   - Notes
//     \__ Implements the quadratic probing method as it has a 
// =====================================================================================
HT_PFX int ClosedHashTable<Key, Val>::Probe(int index) const {
  double a = 0.5;
  double b = 0.5;
  double c = 0;
  
  return (int)(a * (index * index) + b * (index) + c);
}

// =====================================================================================
// 
// =====================================================================================
HT_PFX Val ClosedHashTable<Key, Val>::Find(const Key& k) const {
  int hashed_index = Hash(k);
  if (buffer[hashed_index].key == k) {
    return buffer[hashed_index].val;  
  }
  else {
    for (int i = 0; i < M; i++) {
      int offset = Probe(i);
      if (buffer[(hashed_index + offset) % M].key == k) {
        return buffer[(hashed_index + offset) % M].val;
      }
    }
  }
  throw std::string("Could not find key");
}
HT_PFX void ClosedHashTable<Key,Val>::Insert(const Key& k, const Val& v) {
  int hashed_index = Hash(k);
  bool status = false;
  if (buffer[hashed_index].key == EMPTY) {
    buffer[hashed_index] = typename ClosedHashTable<Key, Val>::Record(k, v);
    status = true;
  }
  else {
    for (int i = 0; i < M; i++) {
      int offset = Probe(i);
      int newIndex = (hashed_index + offset) % M;
      if (buffer[newIndex].key == EMPTY || buffer[newIndex].key == TOMBSTONE) {
        buffer[newIndex] = typename ClosedHashTable<Key, Val>::Record(k, v);
        status = true;
        return;
      }
    }
  }
  if (!status) {
    std::stringstream ss;
    ss << "Failed to insert Record{" << k << ", " << v << "}";
    throw ss.str();
  }
}
HT_PFX void ClosedHashTable<Key, Val>::Remove(const Key& k) {
  int hashed_index = Hash(k);
  if (buffer[hashed_index].key == k) {
    buffer[hashed_index].key = TOMBSTONE;
    size--;
  }
  else {
    for (int i = 0; i < M; i++) {
      int offset = Probe(i);
      int newIndex = (hashed_index + offset) % M;
      if (buffer[newIndex].key == k) {
        buffer[newIndex].key = TOMBSTONE;
        size--;
        return;        
      }
    }
  }
}
HT_PFX int ClosedHashTable<Key, Val>::Size() {
  return size;
}

template <typename CKey, typename CVal>
std::ostream& operator<<(std::ostream& os, const ClosedHashTable<CKey, CVal>& closedHT) {
  for (int i = 0; i < closedHT.M; i++) {
    if (closedHT.buffer[i].key == closedHT.EMPTY) {
      os << '{' << "empty : " << "none }" << std::endl;
    }
    else {
      os << '{' << std::setfill('_') << std::setw(4) << std::left << closedHT.buffer[i].key << ", " << std::setw(4) << closedHT.buffer[i].val << '}' << std::endl;    
    }
  }
  return os;
}
#endif