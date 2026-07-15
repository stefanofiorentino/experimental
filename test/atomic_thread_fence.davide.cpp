#include <gmock/gmock.h>

#include <future>
#include <gtest/gtest.h>
#include <sstream>
#include <thread>

// concurrency by Scott Meyers
template<typename F, typename... Args>
inline auto
reallyAsync(F&& f, Args&&... args)
{
  return std::async(
    std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
}

TEST(atomic_thread_fence, davide)
{
  const int expected = 4;
  std::ostringstream probe;

  std::atomic<int> done = 0;
  int result = -1;

  auto f1 = reallyAsync(
    [&](const int n) {
      result = n;
      std::atomic_thread_fence(std::memory_order_release);

      done.store(n, std::memory_order_relaxed);
    },
    expected);

  auto f2 = reallyAsync([&]() {
    while (done.load(std::memory_order_relaxed) != expected)
      std::this_thread::yield();

    std::atomic_thread_fence(std::memory_order_acquire);

    probe << (result == expected ? "PASSED" : "FAILED");
    ;
  });

  f1.get();
  f2.get();

  ASSERT_EQ("PASSED", probe.str());
}
