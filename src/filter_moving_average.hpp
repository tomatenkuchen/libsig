/**
 * @file filter_moving_average.hpp
 * @brief signal filter with PT1 characteristic
 * @author Timo Werner
 * @date 2023-02-15
 */

#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace sig {

template <typename T>
  requires std::integral<T> || std::floating_point<T>
class MovingAverage {
 public:
  MovingAverage(T initial_value, T scale)
      : state{initial_value * scale}, scale{scale} {}
  MovingAverage(MovingAverage const&) = default;
  MovingAverage(MovingAverage&&) = default;
  MovingAverage& operator=(MovingAverage const&) = default;
  MovingAverage& operator=(MovingAverage&&) = default;

  T update(T value) const {
    state -= state / scale;
    state += value;
    return state / scale;
  }

  void reset(T reset_value) const { state = reset_value * scale; }

  T output() const { return state / scale; }

 private:
  T mutable state;
  T scale;
};

}  // namespace sig
