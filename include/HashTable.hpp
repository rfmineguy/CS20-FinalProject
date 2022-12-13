#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

template <typename Key, typename Val>
class HashTable {
protected:
  struct Record {
    Key key;
    Val val;
    Record(): key(Key()), val(Val()) {}
    Record(Key k, Val v): key(k), val(v) {}
  };

public:
  HashTable() {}
  virtual ~HashTable() {}
  virtual int Hash(const Key&) const = 0;
  virtual int Probe(int) const = 0;
  virtual Val Find(const Key&) const = 0;
  virtual void Insert(const Key&, const Val&) = 0;
  virtual void Remove(const Key&) = 0;
  virtual int Size() = 0;
};

#endif