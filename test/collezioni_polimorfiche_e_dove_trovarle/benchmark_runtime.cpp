#include <benchmark/benchmark.h>

#include <cstddef>
#include <sstream>
#include <string>

#include "light_management/dimmable_light_bulb.hpp"
#include "light_management/include/document.hpp"
#include "light_management/light_bulb.hpp"

static collection_t
build_runtime(size_t count)
{
  collection_t c;
  c.reserve(2 * count);
  for (size_t i = 0; i < count; ++i) {
    c.emplace_back(on_off_light_t());
    c.emplace_back(dimmable_light_t());
  }
  return c;
}

static std::string
draw_runtime(const collection_t& c)
{
  std::ostringstream oss;
  draw(c, oss, 0);
  return oss.str();
}

// ============================================================================
// Benchmark parameterisation: 4 ops x 3 scales = 12 benchmarks
// ============================================================================
static void
BM_Runtime_Create(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto w = build_runtime(n);
    benchmark::DoNotOptimize(w);
  }
}

static void
BM_Runtime_Draw(benchmark::State& state, size_t n)
{
  auto w = build_runtime(n);
  for (auto _ : state) {
    std::string out = draw_runtime(w);
    benchmark::DoNotOptimize(out);
  }
}

static void
BM_Runtime_Switch(benchmark::State& state, size_t n)
{
  auto c = build_runtime(n);
  for (auto _ : state) {
    do_switch(c, false);
    do_switch(c, true);
    benchmark::ClobberMemory();
  }
}

static void
BM_Runtime_Mixed(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto c = build_runtime(n);
    std::string out = draw_runtime(c);
    do_switch(c, false);
    do_switch(c, true);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }
}

// Scales: 100, 10K, 100K
BENCHMARK_CAPTURE(BM_Runtime_Create, small, 100);
BENCHMARK_CAPTURE(BM_Runtime_Create, medium, 10000);
BENCHMARK_CAPTURE(BM_Runtime_Create, large, 100000);

BENCHMARK_CAPTURE(BM_Runtime_Draw, small, 100);
BENCHMARK_CAPTURE(BM_Runtime_Draw, medium, 10000);
BENCHMARK_CAPTURE(BM_Runtime_Draw, large, 100000);

BENCHMARK_CAPTURE(BM_Runtime_Switch, small, 100);
BENCHMARK_CAPTURE(BM_Runtime_Switch, medium, 10000);
BENCHMARK_CAPTURE(BM_Runtime_Switch, large, 100000);

BENCHMARK_CAPTURE(BM_Runtime_Mixed, small, 100);
BENCHMARK_CAPTURE(BM_Runtime_Mixed, medium, 10000);
BENCHMARK_CAPTURE(BM_Runtime_Mixed, large, 100000);

BENCHMARK_MAIN();
