#include <gmock/gmock.h>

#include <future>

class thread_unsafe
{
  bool status;

public:
  thread_unsafe()
    : status(false)
  {
  }
  bool getStatus() const { return status; }

  void setStatus(bool _status) { thread_unsafe::status = _status; }
};

TEST(thread_unsafe, basic)
{
  thread_unsafe ts;
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

TEST(thread_unsafe, thsan)
{
  thread_unsafe ts;
  bool status{};

  ASSERT_FALSE(status);

  auto th1 =
    reallyAsync([](thread_unsafe& _ts) { _ts.setStatus(true); }, std::ref(ts));

  auto th2 = reallyAsync(
    [](thread_unsafe const& _ts, bool& _status) { _status = _ts.getStatus(); },
    std::cref(ts),
    std::ref(status));
}