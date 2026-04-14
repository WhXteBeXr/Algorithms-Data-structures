#pragma once
#include  <vector>

using std::vector;

class IntVector
{
  private:
  size_t size;
  int vectorElement;
  vector<int> numbers;

  public:
  IntVector();
  explicit IntVector(size_t size, int element);
  IntVector(IntVector&& other);
  IntVector& operator=(const IntVector& other);
  IntVector& operator=(IntVector&& other);

  ~IntVector();

  void setSize(size_t size);
  void setVectorElement(int element);

  size_t getSize() const;

  void vectorReserve(int length);
  void vectorShrink();
  void pushBack(int element);
  void popBack();
  void print() const;
};
