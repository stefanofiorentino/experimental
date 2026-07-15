#include <benchmark/benchmark.h>

#include <cstddef>
#include <sstream>
#include <string>

// ============================================================================
// Shared component structs
// ============================================================================
struct on_off_t
{
  bool is_on{ true };
};
struct level_t
{
  size_t level{ 42 };
};

// ---------- DOD entity-locator structs ----------
struct on_off_light_dod_t
{
  size_t on_off_idx;
};
struct dimmable_light_dod_t
{
  size_t on_off_idx;
  size_t level_idx;
};

// ============================================================================
// Data-Oriented Design (DOD)
// ============================================================================
struct world_dod
{
  std::vector<on_off_t> on_offs;
  std::vector<level_t> levels;
  std::vector<on_off_light_dod_t> on_off_lights;
  std::vector<dimmable_light_dod_t> dimmable_lights;

  size_t add_on_off(on_off_t&& o)
  {
    on_offs.emplace_back(std::forward<on_off_t>(o));
    return on_offs.size() - 1;
  }

  size_t add_level(const level_t& l)
  {
    levels.emplace_back(l);
    return levels.size() - 1;
  }

  void add_on_off_light(on_off_t&& o)
  {
    on_off_light_dod_t e;
    e.on_off_idx = add_on_off(std::forward<on_off_t>(o));
    on_off_lights.emplace_back(e);
  }

  void add_dimmable_light(on_off_t&& o, level_t&& l)
  {
    dimmable_light_dod_t e;
    e.on_off_idx = add_on_off(std::forward<on_off_t>(o));
    e.level_idx = add_level(l);
    dimmable_lights.emplace_back(std::move(e));
  }

  void draw(std::ostream& oss) const
  {
    oss << "<?xml version=\"1.0\"?>\n";
    oss << "<document>\n";
    for (const auto& ol : on_off_lights)
      oss << "<on_off_light>\n<is_on>\n"
          << std::boolalpha << on_offs[ol.on_off_idx].is_on
          << "\n</is_on>\n</on_off_light>\n";
    for (const auto& dl : dimmable_lights)
      oss << "<dimmable_light>\n<is_on>\n"
          << std::boolalpha << on_offs[dl.on_off_idx].is_on
          << "\n</is_on>\n<level>\n"
          << levels[dl.level_idx].level << "\n</level>\n"
          << "</dimmable_light>\n";
    oss << "</document>\n";
  }

  void do_switch(bool status)
  {
    for (auto& o : on_offs)
      o.is_on = status;
  }

  void reserve(size_t size)
  {
    on_offs.reserve(size);
    levels.reserve(size);
    on_off_lights.reserve(size);
    dimmable_lights.reserve(size);
  }
};

static world_dod
build_dod(size_t count)
{
  world_dod w;
  w.reserve(2 * count);
  for (size_t i = 0; i < count; ++i) {
    w.add_on_off_light(on_off_t{ true });
    w.add_dimmable_light(on_off_t{ true }, level_t{ 42 });
  }
  return w;
}

// ---------- DOD helpers ----------
static std::string
draw_dod(const world_dod& w)
{
  std::ostringstream oss;
  w.draw(oss);
  return oss.str();
}

// ============================================================================
// Benchmark parameterisation: 4 ops x 3 scales = 12 benchmarks
// ============================================================================
static void
BM_DOD_Create(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto w = build_dod(n);
    benchmark::DoNotOptimize(w);
  }
}

static void
BM_DOD_Draw(benchmark::State& state, size_t n)
{
  auto w = build_dod(n);
  for (auto _ : state) {
    std::string out = draw_dod(w);
    benchmark::DoNotOptimize(out);
  }
}

static void
BM_DOD_Switch(benchmark::State& state, size_t n)
{
  auto w = build_dod(n);
  for (auto _ : state) {
    w.do_switch(false);
    w.do_switch(true);
    benchmark::ClobberMemory();
  }
}

static void
BM_DOD_Mixed(benchmark::State& state, size_t n)
{
  for (auto _ : state) {
    auto w = build_dod(n);
    std::string out = draw_dod(w);
    w.do_switch(false);
    w.do_switch(true);
    benchmark::DoNotOptimize(out);
    benchmark::ClobberMemory();
  }
}

// Scales: 100, 10K, 100K
BENCHMARK_CAPTURE(BM_DOD_Create, small, 100);
BENCHMARK_CAPTURE(BM_DOD_Create, medium, 10000);
BENCHMARK_CAPTURE(BM_DOD_Create, large, 100000);

BENCHMARK_CAPTURE(BM_DOD_Draw, small, 100);
BENCHMARK_CAPTURE(BM_DOD_Draw, medium, 10000);
BENCHMARK_CAPTURE(BM_DOD_Draw, large, 100000);

BENCHMARK_CAPTURE(BM_DOD_Switch, small, 100);
BENCHMARK_CAPTURE(BM_DOD_Switch, medium, 10000);
BENCHMARK_CAPTURE(BM_DOD_Switch, large, 100000);

BENCHMARK_CAPTURE(BM_DOD_Mixed, small, 100);
BENCHMARK_CAPTURE(BM_DOD_Mixed, medium, 10000);
BENCHMARK_CAPTURE(BM_DOD_Mixed, large, 100000);

BENCHMARK_MAIN();
