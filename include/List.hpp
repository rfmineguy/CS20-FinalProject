#ifndef LIST_HPP
#define LIST_HPP

template <typename T>
class List {
public:
  List();
  virtual ~List();
  virtual T At(int index) = 0;
  virtual void Insert(int index, T elem) = 0;
  virtual void Replace(int index, T elem) = 0;
  virtual void Remove(int index) = 0;
  virtual int Length() = 0;
};

#endif