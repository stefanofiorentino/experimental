#include <gmock/gmock.h>
#include <iostream>

using namespace std;

void
alloc_data(int** data, int* count)
{
  *count = 10;
  (*data) = new int[10];
  ;

  std::fill((*data), (*data) + *count, 42);
}

void
free_data(int* data)
{
  delete[] data;
}

void
free_data_and_clear(int** data)
{
  int* p;

  p = *data;
  if (p) {
    *data = nullptr;
    delete[] p;
  }
}
void
use_data(int const* data)
{
  cout << *data << endl;
}

TEST(elvis_2, free_data)
{
  int cnt;
  int* data;

  alloc_data(&data, &cnt);
  use_data(&data[3]);
  free_data(data);

  ASSERT_NE(nullptr, data); // anche se clang-tidy lo becca subito
}

TEST(elvis_2, free_data_and_clear)
{
  int cnt;
  int* data;

  alloc_data(&data, &cnt);
  use_data(&data[3]);
  free_data_and_clear(&data);

  ASSERT_EQ(nullptr, data);
}
