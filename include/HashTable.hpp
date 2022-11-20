#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

template <typename Key, typename Val>
class HashTable {
public:
  HashTable();
  virtual ~HashTable();
  virtual int Hash(const Key&) = 0;
  virtual Val Find(const Key&) = 0;
  virtual void Insert(const Key&, const Val&) = 0;
  virtual void Remove(const Key&) = 0;
  virtual int Size() = 0;
};

#endif