#include "compute.hpp"
#include <iostream>

int main() {
    constexpr std::uint64_t limit = 1'000'000;
    const auto result = perf::sum_arithmetic_series(limit);

    std::cout << "Sum 1.." << limit << " = " << result << '\n';
    return 0;
}
