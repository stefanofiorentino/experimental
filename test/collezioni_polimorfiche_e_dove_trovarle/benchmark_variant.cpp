#include <benchmark/benchmark.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include <light_management/dimmable_light_bulb.hpp>
#include <light_management/include/type_traits.hpp>
#include <light_management/light_bulb.hpp>

// ============================================================================
// Utility trait: does T::do_switch(bool) exist?
// ============================================================================
template<typename T, class = void>
struct has_do_switch_bool_impl : std::false_type
{
};

template<typename T>
struct has_do_switch_bool_impl<
  T,
  decltype(std::declval<T>().do_switch(std::declval<bool>()), void())>
  : std::true_type
{
};

template<typename T>
inline constexpr bool has_do_switch_bool_v = has_do_switch_bool_impl<T>::value;

// ============================================================================
// std::variant (type-safe union with std::visit)
// ============================================================================
using light_variant_t = std::variant<on_off_light_t, dimmable_light_t>;
using variant_collection_t = std::vector<light_variant_t>;

static variant_collection_t
build_variant(size_t count)
{
  variant_collection_t c;
  c.reserve(2 * count);
  for (size_t i = 0; i < count; ++i) {
    c.emplace_back(on_off_light_t{});
    c.emplace_back(dimmable_light_t{});
  }
  return c;
}

static std::string
draw_variant(variant_collection_t& c)
{
  std::ostringstream oss;
  oss << "<?xml version=\"1.0\"?>\n";
  oss << "<document>\n";
  for (auto& light_ : c) {
    std::visit(
      [&oss](auto&& light) {
        if constexpr (has_void_draw_v<decltype(light)>) {
          light.draw(oss, 0);
        }
      },
      light_);
  }
  oss << "</document>\n";
  return oss.str();
}

static void
do_switch_variant(variant_collection_t& c, bool status)
{
  for (auto& light_ : c) {
    std::visit(
      [status](auto&& light) {
        if constexpr (has_do_switch_bool_v<decltype(light)>) {
          light.do_switch(status);
        }
      },
      light_);
  }
}

// ============================================================================
// Benchmark parameterisation: 4 ops x 3 scales = 12 benchmarks
// ============================================================================
static void
BM_Variant_Create(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto c = build_variant(n);
    benchmark::DoNotOptimize(c);
  }
}

static void
BM_Variant_Draw(benchmark::State& state, size_t n)
{
  auto c = build_variant(n);
  for (auto _ : state) {
    std::string out = draw_variant(c);
    benchmark::DoNotOptimize(out);
  }
}

static void
BM_Variant_Switch(benchmark::State& state, size_t n)
{
  auto c = build_variant(n);
  for (auto _ : state) {
    do_switch_variant(c, false);
    do_switch_variant(c, true);
    benchmark::ClobberMemory();
  }
}

static void
BM_Variant_Mixed(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto c = build_variant(n);
    std::string out = draw_variant(c);
    do_switch_variant(c, false);
    do_switch_variant(c, true);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }
}

// Scales: 100, 10K, 100K
BENCHMARK_CAPTURE(BM_Variant_Create, small, 100);
BENCHMARK_CAPTURE(BM_Variant_Create, medium, 10000);
BENCHMARK_CAPTURE(BM_Variant_Create, large, 100000);

BENCHMARK_CAPTURE(BM_Variant_Draw, small, 100);
BENCHMARK_CAPTURE(BM_Variant_Draw, medium, 10000);
BENCHMARK_CAPTURE(BM_Variant_Draw, large, 100000);

BENCHMARK_CAPTURE(BM_Variant_Switch, small, 100);
BENCHMARK_CAPTURE(BM_Variant_Switch, medium, 10000);
BENCHMARK_CAPTURE(BM_Variant_Switch, large, 100000);

BENCHMARK_CAPTURE(BM_Variant_Mixed, small, 100);
BENCHMARK_CAPTURE(BM_Variant_Mixed, medium, 10000);
BENCHMARK_CAPTURE(BM_Variant_Mixed, large, 100000);

BENCHMARK_MAIN();
