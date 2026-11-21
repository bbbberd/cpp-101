#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    // basic stack based data type
    std::int32_t item = 5;          // 32-bit whole number (integer)
    double weight = 12.5;           // Floating point number
    bool exist = true;              // Logical binary value true/false

    // Data with dynamic size is safer in std::vector
    // std::vector is dynamic array compare to normal array where the size is static
    std::vector<double> sample(1000, 1.0);

    std::cout 
        << "Item: " << item 
        << ", Weight: " << weight 
        << ", Exist?: " << (exist ? "yes" : "no") 
        << ", Total sample: " << sample.size() << '\n';

    // Local scope inside the block {..}
    {
        std::int32_t temporary_value = 99;
        std::cout 
            << "Within block -> temporary_value = "
            << temporary_value << '\n';
    }

    // Here temporary_value is no longer available
    return 0;
}

// Here everything inside int main() no longer available
