# C++ Memory Model

## Bit -> Byte -> Data Type

In the most basic, computer only see `0` and `1`, one digit of them is called **bit**.

* **1 byte = 8 bit**: One character of `A` is eating 1 byte. Which is `01000001` in binary machine code, all 8 digit/bit, the decimal number of `A` is `64` in ASCII.

| Data Type | Value | Common Size | Description | 
| --- | --- | --- | --- |
| `bool` | `true`, `false` | 1 byte | Logical condition of true/false |
| `char` | `A`, `9` | 1 byte | Individual character which is 8-bit |
| `int` | `42`, `-15` | 4 byte | Integer number |
| `double` | `3.14` | 8 byte | Floating point number with precision |
| `std::size_t` | `0` to `1024` | 8 byte (int 64-bit machine) | Like an unsigned type, a non negatif index to use as a size to scale the element or byte. |

* **Memory address**: Called RAM is like a sequential of locker with number, every byte in memory have unique number so the CPU understand the location.

* **Variable**: A label in code we created to name the memory so it's easy to read/write for us human.

* **Data Type**: A rule in compiler to know how much **byte** it needs for reservation, and to translate it into binary machine code (`0` and `1`) for machine to understand. Like how `01000001` is translated as `A`.

> Notes: The exact size like how many byte of `int` is platform/architecture dependent, so each hardware and Operating System is probably different, while header we used such as `<cstdint>` give us exact type `std::int32_t` and so on, guaranteed the size in each hardware.

## Scope & Lifetime

Before we go to *stack and heap* understand these:

* **Scope**: An area inside our code (with syntax `{}`) that control variable/value to exist or not for access.

* **Lifetime**: A timeline of the value/object, when it is created in memory until it is destroyed or memory being freed (it's **not** guaranteed ends outside `{}`, for example pointer, we'll get there later).

## Memory Map: Stack vs Heap

There is 2 types of memory in computer that the operating system give us, stack and heap:

```
  STACK (Ordered/Sequential)              HEAP (Unordered/Dynamic)
 ┌─────────────────────────┐            ┌────────────────────────────┐
 │    [ funct view() ]     │            │ [ block 4000 byte: data ]  │
 │     int skor = 100      │            │                            │
 ├─────────────────────────┤            │ [ block 64 byte: config ]  │
 │    [ funct main() ]     │            │                            │
 │     int level = 3       │            │ (flexible size, configured │
 │   ptr -> heap address   │───────────►│  while program is running) │
 └─────────────────────────┘            └────────────────────────────┘
  Automatically in and out,               Must be managed carefully
  fast, limited size                      slow, more spacious, higher cost
                                          allocation
```
**Mental Model**:
* **Stack**:
    * Work like a stack of plate: last in is the first out (the top one) (LIFO - last-in, first-out).
    * Each time a function is called, a **stack frame** (its workspace containing local variables) is pushed onto the stack. When the function ends, the same frame is removed.
    * Fast but low capacity (for example 8MB in most linux system which big data is not good thing to use here).

* **Heap**:
    * A place to reserve more memory than stack, can be done during the program running.
    * More bigger/spacious but the lifetime is controlled manualy, not excatly like the **stack** (hence why we talked about scope/lifetime earlier), it needs `allocator` to reserve the memory, and if the lifetime is not control it can be forever there, after the program ends, or memory leaks.

```
             /\          CPU Registers
            /  \         (Faster access speed, smaller capacity)
           /____\
          /      \       L1 / L2 / L3 Cache
         /________\
        /          \     Main Memory (RAM)
       /____________\
      /              \   Storage (SSD / NVMe)
     /________________\  (Slower access speed, bigger capacity)
```

## Code Practice

```bash
mkdir -p /whatever-folder-name/lesson02/src
cd /whatever-folder-name/lesson02/src
```

`src/main.cpp`:

```cpp
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
```

`CMakeLists.txt`:

```
cmake_minimum_required(VERSION 3.20)
project(lesson02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lesson02 src/main.cpp)

target_compile_options(lesson02 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
)
```

## Industry Notes

* Why `std::vector` and not `new/delete`? because it follows RAII principle to reduce risk of *memory leak* and *double free*. Also it's optimized well, even compare to standard array in some circumtances so industry uses it alot and it shows that you are responsible.

* **Comparison in C++17, 20 and 23**: Basic data types (`bool`, `char`, `int`, `double`, `<cstdint>`) and `std::vector` is consistence in all the three, but `std::span` (c++20) and `std::mdspan` (c++23) syntax don't exist in c++17.

* **Becareful with integer overflow**: If `int` 32-bit accept a value beyond it's maximum it will result in signed integer overflow which is **undefined behaviour** for bigger integer use 64 type such as `std::int64_t` or `std::uint64_t`.

## `sizeof(...)`

This operator give u a machine spesific of your data type size in bytes, to check before you write anything this is good exercise to use.
