#include "gmock/gmock.h"

template<typename T>
struct always_false
{
  static constexpr bool value = false;
};

#define DISPATCHER_TO_GRPC_SETTER(param)                                       \
                                                                               \
  template<typename T, typename V>                                             \
  concept has_set_##param = requires(T t, V v)                                 \
  {                                                                            \
    { t.set_##param(v) };                                                      \
  };                                                                           \
                                                                               \
  template<typename T, typename V>                                             \
  concept has_set_allocated_##param = requires(T t, V v)                       \
  {                                                                            \
    { t.set_allocated_##param(v) };                                            \
  };                                                                           \
                                                                               \
  template<typename T>                                                         \
  concept has_mutable_##param = requires(T t)                                  \
  {                                                                            \
    { t.mutable_##param() };                                                   \
  };                                                                           \
                                                                               \
  template<typename ObjType, typename ValueType>                               \
  void dispatch_grpc_##param##_setter(ObjType& obj, const ValueType& value)    \
  {                                                                            \
    if constexpr (has_set_##param<ObjType, ValueType>) {                       \
      obj.set_##param(value);                                                  \
    } else if constexpr (has_set_allocated_##param<ObjType, ValueType>) {      \
      obj.set_allocated_##param(value);                                        \
    } else if constexpr (has_mutable_##param<ObjType>) {                       \
      obj.mutable_##param()->set_value(value);                                 \
    } else {                                                                   \
      static_assert(always_false<ObjType>::value,                              \
                    "No suitable setter found for " #param);                   \
    }                                                                          \
  }

#define SET_GRPC_VALUE(obj, param, value)                                      \
  dispatch_grpc_##param##_setter(obj, value);

DISPATCHER_TO_GRPC_SETTER(id);

struct object_with_id_t
{
  template<typename T>
  void set_id(T&& /* value */)
  {
  }
};

struct object_wihtout_id_t
{};

TEST(Matteo, MacroAndConcepts)
{
  object_with_id_t object_with_id(0);
  SET_GRPC_VALUE(object_with_id, id, 1);
}
