#pragma once

#include <type_traits>

#include "statically_allocated_arena.hpp"

template<typename T, typename Arena>
struct ArenaAllocator
{
  template<typename U, typename A>
  friend struct ArenaAllocator;

  using value_type = T;
  using pointer = T*;

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  explicit ArenaAllocator(Arena* a) noexcept
    : arena(a)
  {
  }

  template<typename U>
  explicit ArenaAllocator(ArenaAllocator<U, Arena> const& rhs) noexcept
    : arena(rhs.arena)
  {
  }

  pointer allocate(std::size_t n)
  {
    return static_cast<pointer>(arena->allocate(n * sizeof(T), alignof(T)));
  }

  void deallocate(pointer p, std::size_t n) noexcept
  {
    arena->deallocate(p, n * sizeof(T));
  }

  template<typename U>
  bool operator==(ArenaAllocator<U, Arena> const& rhs) const noexcept
  {
    return arena == rhs.arena;
  }

  template<typename U>
  bool operator!=(ArenaAllocator<U, Arena> const& rhs) const noexcept
  {
    return arena != rhs.arena;
  }

  virtual ~ArenaAllocator()
  {
    static_assert(!std::is_same_v<void, T>,
                  "T cannot be void to be ISO C++ code.");
  }

private:
  Arena* arena;
};
