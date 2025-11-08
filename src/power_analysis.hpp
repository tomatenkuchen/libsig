#include <units/isq/si/electric_current.h>
#include <units/isq/si/power.h>
#include <units/isq/si/voltage.h>

using namespace units::isq;
using namespace units::aliases::isq::si;

namespace sig {

/**
 * @brief computes complex power and rms voltage and current from raw data points
 * @param voltages range of voltage data, assumed to be the same size of currents
 * @param currents range of current data, assumed to be the same size of voltages
 * @return struct of rms voltage, rmx current and complex power */
template <typename T>
W<T> compute_active_power(std::vector<V<T>> const& voltages, std::vector<A<T>> const& currents) {
  W<T> P(0);
  for (int i = 0; auto const& v : voltages) {
    P += v * currents[i];
    i++;
  }
  return P / voltages.size();
}

template <typename T>
V<T> rms_voltage(std::vector<V<T>> const& voltages) {
  T v_rms = 0;
  for (auto const& v : voltages) {
    v_rms += (v * v).number();
  }
  v_rms /= voltages.size();
  return V<T>(std::pow(v_rms, 0.5));
}

template <typename T>
A<T> rms_current(std::vector<A<T>> const& currents) {
  T i_rms = 0;
  for (auto const& i : currents) {
    i_rms += (i * i).number();
  }
  i_rms /= currents.size();
  return A<T>(std::pow(i_rms, 0.5));
}

}  // namespace sig
