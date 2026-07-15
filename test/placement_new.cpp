#include <gmock/gmock.h>
#include <memory>

struct Widget
{
  Widget(const char** pctor)
    : ptr(pctor)
  {
    static std::string ctor("ctor");
    *ptr = ctor.c_str();
  }
  virtual ~Widget()
  {
    static std::string dtor("dtor");
    *ptr = dtor.c_str();
  }
  const char** ptr;
};

template<typename D>
concept Deleter = requires(D&& d)
{
  std::forward<D>(d)(nullptr);
};

template<typename _Tp, Deleter _Deleter, typename... _Args>
inline std::unique_ptr<_Tp, _Deleter>
make_placement_unique_ptr(unsigned char* __pointer,
                          _Deleter&& __deleter,
                          _Args&&... __args)
{
  return std::unique_ptr<_Tp, _Deleter>(new (__pointer)
                                          _Tp(std::forward<_Args>(__args)...),
                                        std::forward<_Deleter>(__deleter));
}

TEST(placement_new, unique_ptr)
{

  alignas(Widget) unsigned char buf[sizeof(Widget)];
  const char* probe;
  {
    [[maybe_unused]] auto p = make_placement_unique_ptr<Widget>(
      buf, [](Widget* w) { w->~Widget(); }, &probe); // deleter moved
    ASSERT_STREQ(probe, "ctor");
  }
  ASSERT_STREQ(probe, "dtor");
}