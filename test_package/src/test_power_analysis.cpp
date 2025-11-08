#include <units/format.h>
#include <units/isq/si/electric_current.h>
#include <units/isq/si/frequency.h>
#include <units/isq/si/power.h>
#include <units/isq/si/voltage.h>

#include <algorithm>
#include <cmath>
#include <numbers>
#include <ranges>
#include <vector>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "create_signal.hpp"
#include "power_analysis.hpp"

using namespace units::isq;
using namespace units::aliases::isq::si;

TEST_CASE("power analysis tests", "[P]") {
  // sinus computation
  std::vector<V<double>> const v_signal = create_signal<V<double>>(
      SignalType::Sinus, 200, ms<double>(10), Hz<double>(50), V<double>(325));
  std::vector<A<double>> const i_signal = create_signal<A<double>>(
      SignalType::Sinus, 200, ms<double>(10), Hz<double>(50), A<double>(1.42));

  auto const res_power = sig::compute_active_power(v_signal, i_signal);
  auto const res_voltage = sig::rms_voltage(v_signal);
  auto const res_current = sig::rms_current(i_signal);

  REQUIRE_THAT(res_power.number(), Catch::Matchers::WithinRel(W<double>(230).number(), 0.01));
  REQUIRE_THAT(res_voltage.number(), Catch::Matchers::WithinRel(V<double>(230).number(), 0.01));
  REQUIRE_THAT(res_current.number(), Catch::Matchers::WithinRel(A<double>(1.0).number(), 0.01));

  // long sinus that has a phase delay of 90 degrees, and therefore should
  // result to 0 active power
  std::vector<V<double>> const v_sinus_long = create_signal<V<double>>(
      SignalType::Triangular, 2000, ms<double>(100), Hz<double>(50), V<double>(325));
  std::vector<A<double>> const i_sinus_long_phased = create_signal<A<double>>(
      SignalType::Sinus, 2000, ms<double>(100), Hz<double>(50), A<double>(1.42), 64);
  auto res_power_phased = sig::compute_active_power(v_signal, i_sinus_long_phased);
  REQUIRE_THAT(res_power_phased.number(), Catch::Matchers::WithinAbs(W<double>(0).number(), 0.01));

  // triangle signal
  std::vector<V<double>> const v_triangle = create_signal<V<double>>(
      SignalType::Triangular, 2000, ms<double>(100), Hz<double>(50), V<double>(325));
  auto const triangle_rms_voltage = sig::rms_voltage(v_triangle);
  REQUIRE_THAT(triangle_rms_voltage.number(), Catch::Matchers::WithinAbs(187.6, 1));

  BENCHMARK("power analysis of 10000 samples") {
    return sig::compute_active_power(v_signal, i_signal);
  };
  BENCHMARK("voltage computation of 10000 samples") { return sig::rms_voltage(v_signal); };
  BENCHMARK("current computation of 10000 samples") { return sig::rms_current(i_signal); };
}
