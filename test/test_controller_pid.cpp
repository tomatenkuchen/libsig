#include <limits>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "controller_pid.hpp"

TEST_CASE("integral controller PID", "[PID]") {
  ControllerPID<int>::Limits lim{.max = 1'000'000, .min = -1'000'000};
  ControllerPID controller(100, lim);

  // require the initial output to be 0
  REQUIRE(controller.output() == 0);
  SECTION("p-channel test"){
    // zero input
    REQUIRE(controller.update(0) == 0);
    // normal operation
    REQUIRE(controller.update(100) == 10000);
    // be able to store result
    REQUIRE(controller.output() == 10000);
    // negative operation
    REQUIRE(controller.update(-100) == -10000);
    // upper limit
    REQUIRE(controller.update(20'000) == lim.max);
    // lower limit
    REQUIRE(controller.update(-20'000) == lim.min);
  }

  SECTION("pi-channel") {
    // activate i-channel of controller
    controller.cfg_i = {2, 0};
    // initial input
    REQUIRE(controller.output() == 0);
    // zero input
    REQUIRE(controller.update(0) == 0);
    // normal operation
    REQUIRE(controller.update(100) == 10200);
    // i-channel increment
    REQUIRE(controller.update(100) == 10400);
    // i-channel decrement
    REQUIRE(controller.update(-100) == -9800);
    // upper limit p-limit
    REQUIRE(controller.update(20'000) == lim.max);
    // upper limit i-limit
    for(int i = 0; i < 100; ++i){controller.update(20'000);}
    REQUIRE(controller.output() == lim.max);
    // make sure integral state is preserved
    REQUIRE(controller.update(0) == lim.max);
  }

  BENCHMARK("calculation of controll step with p-channel only"){
    return controller.update(400);
  };
}


TEST_CASE("float controller PID", "[PID]") {
  ControllerPID<double>::Limits lim{.max = 1'000'000.0, .min = -1'000'000.0};
  ControllerPID controller(100.0, lim);

  // require the initial output to be 0
  REQUIRE_THAT(controller.output(), Catch::Matchers::WithinRel(0.0, 0.00001));
  SECTION("create output"){
    // zero input
    REQUIRE_THAT(controller.update(0.0),
                 Catch::Matchers::WithinRel(0.0, 0.00001));
    // normal operation
    REQUIRE_THAT(controller.update(100.0),
                 Catch::Matchers::WithinRel(10'000.0, 0.00001));
    // be able to store result
    REQUIRE_THAT(controller.output(),
                 Catch::Matchers::WithinRel(10'000.0, 0.00001));
    // negative operation
    REQUIRE_THAT(controller.update(-100.0),
                 Catch::Matchers::WithinRel(-10'000.0, 0.00001));
    // respekt output limits
    REQUIRE_THAT(controller.update(100'000'000.0),
                 Catch::Matchers::WithinRel(lim.max, 0.00001));
    // respekt output limits negative
    REQUIRE_THAT(controller.update(-100'000'000.0),
                 Catch::Matchers::WithinRel(lim.min, 0.00001));
  }

  SECTION("pi-channel") {
    // activate i-channel of controller
    controller.cfg_i = {2, 0};
    // initial input
    REQUIRE_THAT(controller.output(), Catch::Matchers::WithinRel(0.0, 0.00001));
    // zero input
    REQUIRE_THAT(controller.update(0), Catch::Matchers::WithinRel(0.0, 0.00001));
    // normal operation
    REQUIRE_THAT(controller.update(100),
            Catch::Matchers::WithinRel(10'200.0, 0.00001));
    // i-channel increment
    REQUIRE_THAT(controller.update(100),
            Catch::Matchers::WithinRel(10'400.0, 0.00001));
    // i-channel decrement
    REQUIRE_THAT(controller.update(-100),
            Catch::Matchers::WithinRel(-9'800.0, 0.00001));
    // upper limit p-limit
    REQUIRE_THAT(controller.update(20'000),
            Catch::Matchers::WithinRel(lim.max, 0.00001));
    // upper limit i-limit
    for (int i = 0; i < 100; ++i) {
      controller.update(20'000);
    }
    REQUIRE_THAT(controller.output(), Catch::Matchers::WithinRel(lim.max, 0.00001));
    // make sure integral state is preserved
    REQUIRE_THAT(controller.update(0), Catch::Matchers::WithinRel(lim.max, 0.00001));
    // lower limit
    controller.reset();
    for (int i = 0; i < 100; ++i) {
      controller.update(-20'000);
    }
    REQUIRE_THAT(controller.output(), Catch::Matchers::WithinRel(lim.min, 0.00001));
    // make sure integral state is preserved
    REQUIRE_THAT(controller.update(0), Catch::Matchers::WithinRel(lim.min, 0.00001));
  }

  BENCHMARK("calculation of controll step with p-channel only") {
    return controller.update(400.0);
  };
}
