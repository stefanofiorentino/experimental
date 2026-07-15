#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>

class Arena
{
  unsigned char* const data;
  std::size_t const size;
  std::size_t offset;

public:
  explicit Arena(std::size_t s) noexcept
    : data(static_cast<unsigned char*>(::operator new(s)))
    , size(s)
    , offset(0)
  {
    memset(data, '\0', s);
  }

  Arena(Arena const&) = delete;
  Arena& operator=(Arena const&) = delete;

  ~Arena() { ::operator delete(data); }

  void* allocate(std::size_t n, std::size_t a)
  {
    offset = (offset + a - 1) / a * a;

    if (offset + n > size) {
      throw std::bad_alloc();
    }

    void* result = data + offset;
    offset += n;
    return result;
  }

  void deallocate(void* p, std::size_t n) noexcept
  {
    unsigned char* d = static_cast<unsigned char*>(p);
    memmove(d, d + n, size - static_cast<size_t>(d - data) - n);
    offset -= n;
    memset(data + offset, '\0', size - offset);
  }
};