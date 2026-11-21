#include "compute.hpp"

namespace perf {
std::uint64_t sum_arithmetic_series(std::uint64_t n) noexcept { return (n * (n + 1)) / 2; }
} // namespace perf
