#include <algorithm>
#include <random>
#include <ranges>
#include <type_traits>
#include <vector>
#include <cstdint>
#include <concepts>

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "filter_moving_average.hpp"

template <typename T>
  requires std::integral<T>
std::vector<T> make_random_vector(
    int size, std::uniform_int_distribution<T> distribution) {
  std::random_device rnd_dev;
  std::mt19937 mersenne_engine{rnd_dev()};
  auto gen = [&distribution, &mersenne_engine]() {
    return distribution(mersenne_engine);
  };
  std::vector<T> random_vector(size);
  std::generate(random_vector.begin(), random_vector.end(), gen);

  return random_vector;
}

TEST_CASE("filter test integer", "[filter]") {
  sig::MovingAverage<int32_t> const ma(0, 128);
  // make sure, the initial state is returned
  REQUIRE(ma.output() == 0);

  SECTION("feed data to zero initialized filter") {
    std::uniform_int_distribution<int> d{1,52};
    const int vector_size = 500;
    auto random_vector = make_random_vector(vector_size, d);

    // feed random data into filter
    std::ranges::for_each(random_vector,
                          [&](int random_number) { ma.update(random_number); });

    REQUIRE_THAT(ma.output(),
                 Catch::Matchers::WithinRel((d.a() + d.b()) / 2, 0.1));
  }

  SECTION("reset filter"){
    const int reset_value = 100;
    ma.reset(reset_value);
    REQUIRE(ma.output() == reset_value);
  }
}

TEST_CASE("benchmarking filter", "[filter][benchmark]") {
  sig::MovingAverage<int32_t> ma_i2(0, 2);
  sig::MovingAverage<int32_t> ma_i7(0, 7);
  sig::MovingAverage<float> ma_f2(0, 2.0);
  sig::MovingAverage<float> ma_f7(0, 7.0);
  sig::MovingAverage<double> ma_d2(0, 2.0);
  sig::MovingAverage<double> ma_d7(0, 7.0);

  REQUIRE(ma_i2.output() == 0);
  REQUIRE(ma_i7.output() == 0);
  REQUIRE(ma_f2.output() == 0);
  REQUIRE(ma_f7.output() == 0);
  REQUIRE(ma_d2.output() == 0);
  REQUIRE(ma_d7.output() == 0);

  BENCHMARK("integral filter with power of 2 strength") {
    return ma_i2.update(2);
  };
  BENCHMARK(
      "integral filter with power of 2 stregth and non power of 2 input") {
    return ma_i2.update(7);
  };
  BENCHMARK(
      "integral filter with non-power of 2 strength and power of 2 input") {
    return ma_i7.update(8);
  };
  BENCHMARK(
      "integral filter with non-power of 2 strength and non power of 2 input") {
    return ma_i7.update(13);
  };
  BENCHMARK("float filter with power of 2 strength") {
    return ma_f2.update(2.0);
  };
  BENCHMARK("float filter with power of 2 strength and non power of 2 input") {
    return ma_f2.update(7.0);
  };
  BENCHMARK("double filter with power of 2 strength") {
    return ma_d2.update(2.0);
  };
  BENCHMARK("double filter with power of 2 strength and non power of 2 input") {
    return ma_d2.update(7.0);
  };
}

TEST_CASE("testing mutability of const filter objects", "[filter][const][mutable]"){
  sig::MovingAverage<int32_t> const filter(0, 128);
  REQUIRE(filter.output() == 0);
  REQUIRE(filter.update(128) == 1);
}
