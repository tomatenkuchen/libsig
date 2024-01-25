#include "libsig.hpp"

int main() {
    pid = sig::PIDController({.1, .2, .3, 100.0, 0.0});
    pid.update(1.0);

    return pid.value();
}
