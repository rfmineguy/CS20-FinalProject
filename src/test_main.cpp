#include "../include/DoubleLinkedStack.hpp"
#include "../include/CirclularQueue.hpp"
#include "../include/ClosedHashTable.hpp"
#include "../include/SortedList.hpp"
#define EMPTY 964193619
#define TOMBSTONE 46647138

template <typename T>
void TestDoubleLinkedStackPop(DoubleLinkedStack<T>& stack, int count) {
  std::cout << "==========================" << std::endl;
  std::cout << "Test DoublyLinkedStack Pop" << std::endl;
  std::cout << "==========================" << std::endl;
  for (int i = 0; i < count; i++) {
    try {
      stack.Pop();
    } catch (std::string& e) {
      std::cerr << "Error: " << e << std::endl;
      continue;
    }
    std::cout << "Pop #" << i << " had no error" << std::endl;
  }
  std::cout << "==========================" << std::endl;
}

template <typename T>
void TestDoubleLinkedStackPush(DoubleLinkedStack<T>& stack, int count) {
  std::cout << "==========================" << std::endl;
  std::cout << "Test DoublyLinkedStack Pop" << std::endl;
  std::cout << "==========================" << std::endl;
  for (int i = 0; i < count; i++) {
    try {
      stack.Push(i);
    } catch (std::string& e) {
      std::cerr << "Error: " << e << std::endl;
      continue;
    }
    std::cout << "Push #" << i << " had no error" << std::endl;
  }
  std::cout << "==========================" << std::endl;
}

void TestCircularQueueEnqueue(CircularQueue<std::string>& queue, std::string s) {
  std::cout << "==========================" << std::endl;
  std::cout << "Test CircularQueue Enqueue" << std::endl;
  std::cout << "==========================" << std::endl;
  try {
    queue.Enqueue(s);
  } catch (std::string& e) {
    std::cerr << "Error: " << e << std::endl;
  }
  std::cout << "==========================" << std::endl;
}

int main() {
  DoubleLinkedStack<int> stack;
  TestDoubleLinkedStackPush(stack, 30);
  std::cout << "Stack Size: " << stack.Size() << std::endl;
  std::cout << "Stack: " << stack;
  TestDoubleLinkedStackPop(stack, 31);
  std::cout << "Stack Size: " << stack.Size() << std::endl;
  std::cout << "Stack: " << stack;
  
  TestDoubleLinkedStackPush(stack, 30);
  DoubleLinkedStack<int> stack2 = stack;
  std::cout << "Stack2: " << stack2;
  
  CircularQueue<std::string> queue;
  TestCircularQueueEnqueue(queue, "a");
  TestCircularQueueEnqueue(queue, "b");
  TestCircularQueueEnqueue(queue, "c");
  std::cout << "Queue: " << queue << std::endl;
  
  std::cout << queue.Front() << std::endl;
  queue.Dequeue();
  std::cout << queue.Front() << std::endl;
  queue.Dequeue();
  std::cout << queue.Front() << std::endl;
  
  ClosedHashTable<int, int> ht(EMPTY, TOMBSTONE);
  std::cout << ht;
  for (int i = 0; i < 9; i++) {
    try {
      ht.Insert(i*2, i*3);
    } catch (std::string& e) {
      std::cerr << e << std::endl;
      break;
    }
  }
  std::cout << ht;
  
  // The rule being <= or >= is important, otherwise you can't store duplicates
  SortedList<int> list([](int a, int b) { return a <= b; }, -INT_MAX, INT_MAX);
  list.Insert(4);
  std::cout << list;
  list.Insert(7);
  std::cout << list;
  list.Insert(3);
  std::cout << list;
  list.Insert(8);
  std::cout << list;
  list.Insert(3);
  std::cout << list;
  list.Insert(1);
  std::cout << list;
  list.Insert(1);
  std::cout << list;
  std::cout << list.Length();
}