#include <exception>
#include <gmock/gmock.h>

#include <future>
#include <memory>
#include <mutex>
#include <thread>

class counter_t
{
  size_t value{};

public:
  counter_t() = default;
  explicit counter_t(size_t value_)
    : value(value_)
  {
  }
  void inc() { value++; }
  size_t count() const { return value; }
};

template<class T, class Owner>
class ExternallyLocked
{
  T obj_;

public:
  ExternallyLocked() = default;
  explicit ExternallyLocked(const T& obj)
    : obj_(obj)
  {
  }
  T& Get(std::unique_lock<Owner>&) { return obj_; }
};

class thread_safe_counter_manager_t
{
  mutable std::mutex mtx_;
  ExternallyLocked<counter_t, thread_safe_counter_manager_t> counter;

public:
  void lock() const { mtx_.lock(); }
  void unlock() const { return mtx_.unlock(); }
  void inc()
  {
    std::unique_lock<thread_safe_counter_manager_t> ul(*this);
    counter.Get(ul).inc();
  }
  size_t count()
  {
    std::unique_lock<thread_safe_counter_manager_t> ul(*this);
    return counter.Get(ul).count();
  }
};

TEST(thread_safety, unsafe_counter_inc)
{
  counter_t counter;
  auto th1 = std::jthread([&counter]() { counter.inc(); });
  auto th2 = std::jthread([&counter]() { ASSERT_EQ(1, counter.count()); });
}

TEST(thread_safety, safe_counter_inc)
{
  thread_safe_counter_manager_t thread_safe_counter_manager;
  auto th1 = std::jthread(
    [&thread_safe_counter_manager]() { thread_safe_counter_manager.inc(); });
  auto th2 = std::jthread([&thread_safe_counter_manager]() {
    ASSERT_EQ(1, thread_safe_counter_manager.count());
  });
}
