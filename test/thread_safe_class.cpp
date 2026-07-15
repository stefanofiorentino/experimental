#include <gmock/gmock.h>

#include <future>

class thread_safe
{
  bool status;
  mutable std::mutex mtx; // concurrency got wrong by Sean Parent (efficiency)

public:
  thread_safe()
    : status(false)
  {
  }
  bool getStatus() const
  {
    std::lock_guard<std::mutex> lg{ mtx };
    return status;
  }

  void setStatus(bool _status)
  {
    std::lock_guard<std::mutex> lg{ mtx };
    thread_safe::status = _status;
  }
};

TEST(thread_safe, basic)
{
  thread_safe ts;
  ASSERT_FALSE(ts.getStatus());
}

// concurrency by Scott Meyers
template<typename F, typename... Args>
inline auto
reallyAsync(F&& f, Args&&... args)
{
  return std::async(
    std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
}

TEST(thread_safe, thsan)
{
  thread_safe ts;
  bool status{};

  ASSERT_FALSE(status);

  auto th1 =
    reallyAsync([](thread_safe& _ts) { _ts.setStatus(true); }, std::ref(ts));

  auto th2 = reallyAsync(
    [](thread_safe const& _ts, bool& _status) { _status = _ts.getStatus(); },
    std::cref(ts),
    std::ref(status));
}