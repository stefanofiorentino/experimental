#include <gmock/gmock.h>

#include <cstring>
#include <exception>
#include <memory>
#include <string>

namespace details {
std::string
print_foo()
{
  return "char* details::print_foo()";
}
std::string
print_bar()
{
  return "char* details::print_bar()";
}
} // namespace details

std::string
perform_action_based_on_type(std::string const& action_to_perform)
{
  if ("foo" == action_to_perform) {
    return details::print_foo();
  } else if ("bar" == action_to_perform) {
    return details::print_bar();
  }
  throw std::runtime_error("");
}

TEST(from_switch_case_to_polymorphism, C_style_polymorphism)
{
  ASSERT_EQ("char* details::print_foo()", perform_action_based_on_type("foo"));
  ASSERT_EQ("char* details::print_bar()", perform_action_based_on_type("bar"));
  ASSERT_THROW(perform_action_based_on_type(""), std::runtime_error);
}

class base
{
protected:
  virtual std::string do_print() const = 0;

public:
  std::string print() const { return do_print(); }
  virtual ~base() = default;
};
class crtp_nvi_foo_t : public base
{
  std::string do_print() const override { return details::print_foo(); }
};

class bar : public base
{
  std::string do_print() const override { return details::print_bar(); }
};

std::shared_ptr<base>
factory(std::string const& action_to_perform)
{
  if ("foo" == action_to_perform) {
    return std::make_shared<crtp_nvi_foo_t>();
  } else if ("bar" == action_to_perform) {
    return std::make_shared<bar>();
  }
  throw std::runtime_error("");
}

TEST(from_switch_case_to_polymorphism, Cpp_style_polymorphism)
{
  ASSERT_EQ("char* details::print_foo()", factory("foo")->print());
  ASSERT_EQ("char* details::print_bar()", factory("bar")->print());
  ASSERT_THROW(factory(""), std::runtime_error);
}
