// Adam Shaar
// ashaar2
//
// PriorityQueue.h
//
// custom implementation of PriorityQueue Class
#pragma once

#include "BinaryTree.h"
#include <utility>
#include <vector>

class PriorityQueue {
public:
  PriorityQueue();
  bool isEmpty() const;
  int size() const;
  void insert(BinaryTree * tree, int priority);
  int min() const;
  BinaryTree *removeMin();

private:
  std::vector<std::pair<BinaryTree *, int>> data;
  void bubbleUp(int index);
  void bubbleDown(int index);
  int getParentIndex(int index) const;
  int getLeftChildIndex(int index) const;
  int getRightChildIndex(int index) const;
};
// Default constructor initializes an empty priority queue.
PriorityQueue::PriorityQueue() {}

// Returns true if the priority queue is empty.
bool PriorityQueue::isEmpty() const { return data.empty(); }

// Returns the number of elements in the priority queue.
int PriorityQueue::size() const { return data.size(); }

// Inserts an element with a given priority.
void PriorityQueue::insert(BinaryTree * tree, int priority) {
  data.push_back(std::make_pair(tree, priority));
  bubbleUp(data.size() - 1);
}

// Returns the minimum priority value.
int PriorityQueue::min() const {
  if (isEmpty()) {
    return -1;
  }
  return data.front().second;
}

// Removes and returns the element with the minimum priority.
BinaryTree *PriorityQueue::removeMin() {
  if (isEmpty()) {
    return nullptr;
  }
  BinaryTree *result = data.front().first;
  data.front() = data.back();
  data.pop_back();
  bubbleDown(0);
  return result;
}

// Reorders the priority queue after insertion.
void PriorityQueue::bubbleUp(int index) {
  while (index > 0 && data[getParentIndex(index)].second > data[index].second) {
    std::swap(data[getParentIndex(index)], data[index]);
    index = getParentIndex(index);
  }
}

// Reorders the priority queue after removing the minimum element.
void PriorityQueue::bubbleDown(int index) {
  int minIndex = index;
  int leftChildIndex = getLeftChildIndex(index);
  int rightChildIndex = getRightChildIndex(index);

  if (leftChildIndex < size() &&
      data[leftChildIndex].second < data[minIndex].second) {
    minIndex = leftChildIndex;
  }

  if (rightChildIndex < size() &&
      data[rightChildIndex].second < data[minIndex].second) {
    minIndex = rightChildIndex;
  }

  if (minIndex != index) {
    std::swap(data[index], data[minIndex]);
    bubbleDown(minIndex);
  }
}

// Returns the parent index of a given index.
int PriorityQueue::getParentIndex(int index) const { return (index - 1) / 2; }

// Returns the left child index of a given index.
int PriorityQueue::getLeftChildIndex(int index) const { return 2 * index + 1; }

// Returns the right child index of a given index.
int PriorityQueue::getRightChildIndex(int index) const { return 2 * index + 2; }
