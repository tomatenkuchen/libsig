#include "libsig.h"
#include <vector>
#include <string>

int main() {
    libsig();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    libsig_print_vector(vec);
}
