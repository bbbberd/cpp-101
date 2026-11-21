# Pointer and Reference

## Variable & Function

When you initialize variable with value of other variable:

```cpp
int number = 5;
int second_number = number;

// equivalent to:
// int number = 5;
// int second_number = 5;
```

What it does is ***copy***, the value is duplicate not transfered (in stack). To use one value for several variable, we should use **pointer/reference**.

* for `int`(4 byte) and `bool`(1 byte), copying is cheap for performance.
* but with big *buffer* like *array* with hundreds or thousands of value, this makes the program slow.

This applied for `pass by value` function too.

**Pointer Mental Model**:

```
    variable x
    +--------+
    |   42   |      memory address: 0x7f..
    +--------+


    variable p (pointing variabel x)
    +--------+
    | 0x7f.. |      memory address: 0x78tc..
    +--------+   
```

**Reference Mental Model**:

```
    Variable (Value)                   Pointer               Reference

                                  (Separate variable       (Alias of the same
                                  but pointing other       variable, not standalone,
                                  variable)                not pointing any)
 ┌─────────────────────┐       ┌────────────────────┐    ┌─────────────────────┐
 │ name: number        │       │ name: p_number     │    │ name: ref_number    │
 │ address: 0x7ffc1000 │◄────┐ │ address: 0x7bc4... │    │ address: x7ffc1000  │
 │ value: 100          │     └─│ value: 0x7ffc1000  │    │ value: 100          │
 └─────────────────────┘       └────────────────────┘    └─────────────────────┘
```

Syntax:

```cpp
int number = 5;                 // normal variable
int *p_number = &number;        // variable pointer, pointing to variable 
int &ref_number = number;       // variable reference, aliasing variable name
```

> Think of reference as ghost variable, they don't exist.

* **Pointer variable**: which starts with `*` called dereference, is a variable that accept only an address, so it can't hold value directly. And its not limited only for `&` but **any valid memory address of the same data type**.

| Syntax | Valid? | Why? |
| --- | --- | --- |
| `int* p = &number;` | Yes | &number fetches the memory address of number (`int*`). |
| `int* p = p1;` | Yes | `p1` is already an `int*` (holds a valid memory address). |
| `int* p = nullptr;` | Yes | `nullptr` is the standard, safe way to represent an empty address. |
| `int* p = arr;` | Yes | An array's name automatically converts to the address of its first element (`&arr`). |
| `int* p = new int(42);` | Yes | new allocates memory on the heap and returns its address (`int*`). |
| `int* p = p1 + 1;` |Yes | Pointer arithmetic shifts the address by one `int` size (useful in arrays). |
| `int* p = number;` | No (Compiler Error) | number is a plain `int` value (`42`), not a memory address. |
| `int* p = *p1;` | No (Compiler Error) | `*p1` dereferences the pointer, giving you the int value (`42`), not an address. |
| `int* p = &p1;` | No (Compiler Error) | `&p1` is the address of a pointer (`int**`), not the address of an int (`int*`). |
| `double* p = &number;` | No (Compiler Error) | Type mismatch: a double* pointer cannot store the address of an `int`. |

* **Reference variable**: Which start with `&` (address-of) means taking the address instead of value.
    * **Must be initialized** immediately, unlike pointer you can't leave them empty `int &ref;` is unvalid.
    * **Cannot reset** or **Changed**, once it sets `int &ref = var` its permanent.

| Syntax | Valid? | Why? |
| --- | --- | --- |
| `int& ref = number;` | Yes | `number` is an `int` variable. `ref` becomes another name for number. |
| `int& ref = *p;` | Yes | `*p` dereferences the pointer, giving you the underlying `int` variable. `ref` aliases that same variable. |
| `int& ref = arr[0];` | Yes | `arr[0]` is a specific `int` element inside the array. |
| `int& ref2 = ref;` | Yes | You can create a reference to another existing reference (both alias `number`). |
| `const int& ref = 42;` | Yes (Special Case) | A `const` reference is allowed to bind to a temporary literal value (`42`). |
| `int& ref = &number;` | No (Compiler Error) | `&number` is a memory address (`int*`), not an `int` variable. A reference to an `int` cannot bind to an address. |
| `int& ref = p1;` | No (Compiler Error) | `p1` is a pointer (`int*`), not an `int` variable. |
| `int& ref = nullptr;` | No (Compiler Error) | References cannot be null; they must always alias a real object. |
| `int& ref;` | No (Compiler Error) | Uninitialized reference: you must assign a variable to a reference the moment you declare it. |
| `int& ref = 42;` | No (Compiler Error) | Non-const references cannot bind to a literal number (`42`) because literals don't have a modifiable variable location. |
| `double& ref = number;` | No (Compiler Error) | Type mismatch: a `double&` reference cannot alias an `int` variable. |

## Code Practice

Create the folder as usual with `main.cpp` inside `src/` folder and `CMakeLists.txt outside` `src/` folder, and write this (don't copy paste, try it, get used to type):

`src/main.cpp`

```cpp
#include <cstdint>
#include <iostream>

// Receiving reference non-const: can change value of the caller without copy
void add_bonus(std::int32_t& value, std::int32_t bonus) noexcept {
    value += bonus;
}

// Receiving const reference: only read, no copy (pretty common for big data)
void print_status(const std::int32_t& value) {
    std::cout << "Value now: " << value << '\n';
}

// Receiving pointer that can be null (opsional / no target)
void penalty_if_any(std::int32_t* target, std::int32_t penalty) noexcept {
    if (target == nullptr) {
        return;  // Safe, no object changed
    }
    *target -= penalty;
}

int main() {
    std::int32_t score = 100;

    // Reference as alias of `score`
    std::int32_t& alias_score = score;
    alias_score += 25;
    print_status(score);  // 125

    // Changing through function as reference
    add_bonus(score, 15);
    print_status(score);  // 140

    // Pointer to score
    std::int32_t* ptr_score = &score;
    std::cout << "Score address: " << static_cast<const void*>(ptr_score)
              << " | Value through *ptr: " << *ptr_score << '\n';

    penalty_if_any(ptr_score, 10);
    print_status(score);  // 130

    // Example of empty pointer (nullptr)
    std::int32_t* ptr_empty = nullptr;
    penalty_if_any(ptr_empty, 50);
    print_status(score);  // Stay 130

    return 0;
}
```

`CMakeLists.txt`:

```cpp
cmake_minimum_required(VERSION 3.20)
project(Lesson03_PointersRefs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pointers_refs src/main.cpp)

target_compile_options(pointers_refs PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
)
```

Compile and build:

```bash
cmake -B build-gcc -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build-gcc && ./build-gcc/pointers_refs

cmake -B build-clang -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld" -DCMAKE_BUILD_TYPE=Debug
cmake --build build-clang && ./build-clang/pointers_refs
```

## Industry notes

* **When to use `const T&`?** for small primitive type (`int`, `bool`, `double`, pointer). 
* *pass-by-value* is usually prefered because it goes directly to *register* without *memory indirection*
* For big object (such as `std::string` or complex data structures), `const T&` is good to avoid copy.
* **When to use pointer?** when the value can be void/empty (`nullptr`), when working with API C/system/GPU, where data need *re-seating* address.
* **Comparison 17/20/23 standard**: pointer and reference are the same, but to see the slice of array in safe way without raw pointer coupled with manual size, c++20 had `std::span` for that (we will explain this later in container topic).
* Try using `-fsanitize=undefined` flag (UBSan) in Clang/Gcc, because program trying to dereference `nullptr`, UBSan will report *null pointer dereference* rather than letting corrupt silently.
