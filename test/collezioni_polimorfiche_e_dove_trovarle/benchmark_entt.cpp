#include <benchmark/benchmark.h>

#include <cstddef>
#include <sstream>
#include <string>

#include <entt/entt.hpp>

// ============================================================================
// Shared component structs
// ============================================================================
struct on_off_t { bool is_on{true}; };
struct level_t { size_t level{42}; };
struct drawable_t {};

// ============================================================================
// EnTT ECS
// ============================================================================
static entt::registry build_entt(size_t count) {
  entt::registry reg;
  for (size_t i = 0; i < count; ++i) {
    auto a = reg.create();
    reg.emplace<drawable_t>(a);
    reg.emplace<on_off_t>(a, true);
    auto b = reg.create();
    reg.emplace<drawable_t>(b);
    reg.emplace<on_off_t>(b, true);
    reg.emplace<level_t>(b, level_t{static_cast<size_t>(42)});
  }
  return reg;
}

static std::string draw_entt(entt::registry& reg) {
  std::ostringstream oss;
  oss << "<?xml version=\"1.0\"?>\n";
  oss << "<document>\n";
  reg.view<drawable_t, on_off_t>(entt::exclude<level_t>).each(
      [&oss](auto, auto& on_off) {
        oss << "<on_off_light>\n<is_on>\n" << std::boolalpha
            << on_off.is_on << "\n</is_on>\n</on_off_light>\n";
      });
  reg.view<on_off_t, level_t>().each(
      [&oss](auto, auto& on_off, auto& level) {
        oss << "<dimmable_light>\n<is_on>\n" << std::boolalpha
            << on_off.is_on << "\n</is_on>\n<level>\n" << level.level
            << "\n</level>\n</dimmable_light>\n";
      });
  oss << "</document>\n";
  return oss.str();
}

// ============================================================================
// Benchmark parameterisation: 4 ops x 3 scales = 12 benchmarks
// ============================================================================
static void BM_Entt_Create(benchmark::State& state, size_t n) {
  for (auto _ : state) {
    auto reg = build_entt(n);
    benchmark::DoNotOptimize(reg);
  }
}

static void BM_Entt_Draw(benchmark::State& state, size_t n) {
  auto reg = build_entt(n);
  for (auto _ : state) {
    std::string out = draw_entt(reg);
    benchmark::DoNotOptimize(out);
  }
}

static void BM_Entt_Switch(benchmark::State& state, size_t n) {
  auto reg = build_entt(n);
  for (auto _ : state) {
    reg.view<on_off_t>().each([](auto, auto& on_off) {
      on_off.is_on = false;
    });
    reg.view<on_off_t>().each([](auto, auto& on_off) {
      on_off.is_on = true;
    });
    benchmark::ClobberMemory();
  }
}

static void BM_Entt_Mixed(benchmark::State& state, size_t n) {
  for (auto _ : state) {
    auto reg = build_entt(n);
    std::string out = draw_entt(reg);
    reg.view<on_off_t>().each([](auto, auto& on_off) {
      on_off.is_on = false;
    });
    reg.view<on_off_t>().each([](auto, auto& on_off) {
      on_off.is_on = true;
    });
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }
}

// Scales: 100, 10K, 100K
BENCHMARK_CAPTURE(BM_Entt_Create, small, 100);
BENCHMARK_CAPTURE(BM_Entt_Create, medium, 10000);
BENCHMARK_CAPTURE(BM_Entt_Create, large, 100000);

BENCHMARK_CAPTURE(BM_Entt_Draw, small, 100);
BENCHMARK_CAPTURE(BM_Entt_Draw, medium, 10000);
BENCHMARK_CAPTURE(BM_Entt_Draw, large, 100000);

BENCHMARK_CAPTURE(BM_Entt_Switch, small, 100);
BENCHMARK_CAPTURE(BM_Entt_Switch, medium, 10000);
BENCHMARK_CAPTURE(BM_Entt_Switch, large, 100000);

BENCHMARK_CAPTURE(BM_Entt_Mixed, small, 100);
BENCHMARK_CAPTURE(BM_Entt_Mixed, medium, 10000);
BENCHMARK_CAPTURE(BM_Entt_Mixed, large, 100000);

BENCHMARK_MAIN();
