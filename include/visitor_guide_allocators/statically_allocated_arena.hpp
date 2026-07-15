#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>

template<std::size_t size>
class Arena
{
  unsigned char const data[size];
  std::size_t offset;

public:
  explicit Arena() noexcept
    : data()
    , offset(0)
  {
    memset(const_cast<unsigned char*>(data), '\0', size);
  }

  Arena(Arena const&) = delete;
  Arena& operator=(Arena const&) = delete;

  void* allocate(std::size_t n, std::size_t a)
  {
    offset = (offset + a - 1) / a * a;

    if (offset + n > size) {
      throw std::bad_alloc();
    }

    void* result = const_cast<unsigned char*>(data) + offset;
    offset += n;
    return result;
  }

  void deallocate(void* p, std::size_t n) noexcept
  {
    unsigned char* d = static_cast<unsigned char*>(p);
    assert(d >= data);
    memmove(d, d + n, size - static_cast<size_t>(d - data) - n);
    offset -= n;
    memset(const_cast<unsigned char*>(data) + offset, '\0', size - offset);
  }
};