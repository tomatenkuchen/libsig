/**
 * @file signal.hpp
 * @brief signal creation from parameter
 */

#pragma once

#include <units/isq/si/frequency.h>
#include <units/isq/si/time.h>

#include <algorithm>
#include <cmath>
#include <numbers>
#include <ranges>
#include <stdexcept>
#include <vector>

using namespace units::isq;
using namespace units::aliases::isq::si;

enum class SignalType { Sinus, Rectangular, Triangular };

template <typename T>
constexpr std::vector<T> create_signal(SignalType t, int number_of_samples, ns<double> duration,
                                       Hz<double> f, T amplitude, uint8_t phase = 0) {
  int n = 0;
  auto filler_sinus = [&]() mutable {
    s<double> const t_sample = duration * n / number_of_samples + 1 / f * phase / 256;
    n++;
    auto const res = amplitude * std::sin((f * t_sample * 2 * std::numbers::pi).number());
    return res;
  };

  auto filler_rectangular = [&]() mutable {
    s<double> const t_period = 1 / f;
    s<double> const t_sample = duration * n / number_of_samples + t_period * phase / 256;
    n++;
    auto const t_mod = std::fmod(t_sample.number(), t_period.number());
    return (t_mod / t_period).number() >= 0.5 ? amplitude : -amplitude;
  };

  auto filler_triangular = [&]() mutable {
    s<double> const t_period = 1 / f;
    s<double> const t_sample = duration * n / number_of_samples + t_period * phase / 256;
    n++;
    auto const t_mod = std::fmod(t_sample.number(), t_period.number());
    auto const triangle = std::abs(t_mod - (t_period.number() / 2)) - t_period.number() / 4;
    auto const scaled_triangle = triangle * 4 / t_period.number() * amplitude;
    return scaled_triangle;
  };

  std::vector<T> sig(number_of_samples);

  switch (t) {
    case SignalType::Sinus:
      std::ranges::generate(sig, filler_sinus);
      break;
    case SignalType::Rectangular:
      std::ranges::generate(sig, filler_rectangular);
      break;
    case SignalType::Triangular:
      std::ranges::generate(sig, filler_triangular);
      break;
    default:
      throw std::runtime_error("used irregular signal shape");
      break;
  }

  return sig;
}
