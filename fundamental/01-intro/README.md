# Intro to C++

## Setup

Install the toolchain using WSL-Debian in Windows or Debian-based distro in Linux (apt).

```bash
sudo apt update && sudo apt install -y \
    build-essential \
    g++ \
    clang \
    llvm \
    lld \
    cmake \
    ninja-build \
    gdb
```

* `build-essential` & `g++`: Compiler GNU collection for using C/C++ header.
* `clang` & `llvm`: LLVM based compiler that are commonly used in industry.
* `lld`: LLVM linker that are more modern and faster than `ld` from GNU.
* `cmake` & `ninja-build`: Build system with industry standard, CMake create build instruction while **Ninja** executing it in paralel with minimal *overhead* (faster than `make`).
* `gdb`: Debugger to disecting CPU register and memori from the program.

verify the installation by typing:

```bash
g++ --version
clang++ --version
cmake --version
ninja --version
```

## C++ Anatomy

Newbies tend to think that `g++ main.cpp` is the way but in cpp production that is so rare to use, there are ***4 steps*** programmers needs to know and often used, because in cpp there are `compile time` and `linker error`:

```
[ main.cpp ]   [ math.cpp ]        (Source Code)
       │              │
       ▼              ▼
┌────────────────────────────┐
│ 1. PREPROCESSOR (cpp)      │ ──► Expansion #include, #define, #ifdef
└────────────────────────────┘     Output: Translation Unit (.ii)
       │              │
       ▼              ▼
┌────────────────────────────┐
│ 2. COMPILER (cc1plus/clang)│ ──► Parsing AST, IR Optimization, Codegen
└────────────────────────────┘     Output: Assembly (.s)
       │              │
       ▼              ▼
┌────────────────────────────┐
│ 3. ASSEMBLER (as)          │ ──► Translating mnemonic to byte machine
└────────────────────────────┘     Output: Object File (.o / ELF)
       │              │
       └──────┬───────┘
              ▼
┌────────────────────────────┐
│ 4. LINKER (ld / lld)       │ ──► Symbol resolution, section combination,
└────────────────────────────┘     memoy address relocation
              │
              ▼
      [ app_binary ]                 (Executable ELF di Linux)
```

Explanation:

**1. Preprocessing (`-E`)**:
* Prepocessor don't understand C++ syntax, she is purely manipulating text.
* Every `#include` will be copied and pasted of all content from the header file into your `main.cpp`.
* All combination of `.cpp` and `#include` is called ***Translation Unit (TU)***.
* *Performance Implication*: More header that are included, means more big of the TU that are parsed by the compiler every time there is something being changed.

**2. Compilation (`-S`)**:

* Compiler take one TU (without knowing other `.cpp` file).
* Building ***AST (Abstract Syntax Tree)***, turn it into ***IR (Intermediate Representation)*** like *GIMPLE* in GCC or *LLVM IR* in CLANG then process the *optimization passes* (like *inlining*, *loop unrolling*, *dead code elimination*).
* The output will be Assembly instruction script (`.s`) spesific to the target machine architecture of the CPU (for example of mine is `x86_64`).

**3. Assembly (`-c`)**:

* Translating assembly instruction above into *machine code binary* as ***Object File***(`.o`) format.
* The `.o` is recipe for the ***ELF (Executable and Linkable Format)***. Inside there is a function symbol that are defined (defined symbol) and external function where the address are not yet known (undefined symbols).

**4. Linking**:
* Linker collecting all the file `.o` and external library (`.a` for *static library* in linux and `.lib` for Windows, `.so` for *shared library* in Linux, or `.dll` in Windows)
* The whole job of linker is for **Symbol Resolution** (matching the function with the definition) and **Relocation** (to set the final virtual address for the binary).
* If the function is declared in `.hpp` header, compiler will pass the part 2, ***but*** if it's forgot to be linked with the implementation, ***linker*** will give classic error warning `undefined reference to ..`
* The end product is ELF file in Linux, or EXE file in Windows.

## Project Structure and CMake

create a project:

```bash
mkdir -p /whatever-your-project-name/lesson-01-intro/src
cd /whatever-your-project-name/lesson-01-intro
```
We will create **3 file** for exercise, to give u mental image of how c++ work under the hood, create:

`src/compute.hpp` (contract/declaration):

```cpp
#pragma once
#include <cstdint>

namespace perf {
    // function declaration: telling the compiler about function signature
    // without forcing to compile the body in every file.
[[nodiscard]] std::uint64_t sum_arithmetic_series(std::uint64_t n) noexcept;
}
```

`src/compute.cpp` (implementation - 1st translation unit):

```cpp
#include "compute.hpp"

namespace perf {
    std::uint64_t sum_arithmetic_series(std::uint64_t n) noexcept {
        // O(1) optimization compare to O(N) loop: basic example of algorithmic optimization
        return (n * (n + 1)) / 2;
    }
}
```

src/main.cpp (Entry point - 2nd translation unit):

```cpp
#include <iostream>
#include "compute.hpp"

int main(){
    constexpr std::uint64_t limit = 1'000'000;
    const auto result = perf::sum_arithmetic_series(limit);

    std::cout << "Sum 1.." << limit << " = " << result << '\n';
    return 0;
}
```


`CMakeLists.txt` (Build configuration in industry):

```cmake
cmake_minimum_required(VERSION 3.20)
project(anatomy_cpp LANGUAGES CXX)

# Lock on C++20 standard without non standard compiler extension
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create compile_commands.json so the LSP (clangd in neovim/VSCode) remain accurate
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(anatomy_cpp
    src/main.cpp
    src/compute.cpp
)

# Strict warning flags which is a must in performance & systems project
target_compile_options(anatomy_cpp PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
)
```

Now we build the project with **2 directory build** separated, one is GCC and other is CLANG, using Ninja:

```bash
# Build 1: For GCC (g++)
cmake -B build-gcc -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-gcc
./build-gcc/anatomy_cpp

# Build 2: For Clang (clang++) + LLD Linker
cmake -B build-clang -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld" -DCMAKE_BUILD_TYPE=Release
cmake --build build-clang
./build-clang/anatomy_cpp
```

Build it manually to see the ***4 steps***:

**1. Prepocessing**

```bash
clang++ -std=c++19 -E src/main.cpp -o main.ii && wc -l main.ii
```
> You'll see how many thousand of code line that are produced only in one header `iostream`

**2. Compiling**

```bash
clang++ -std=c++19 -O2 -S src/compute.cpp -o compute.s
clang++ -std=c++19 -O2 -S src/main.cpp -o main.s
```
> Try with or without optimization level 2 (`O2`) and see the different in the `.s` file

**3. Assembling**

```bash
# manual
clang++ -std=c++20 -O2 -c src/compute.cpp -o compute.o
clang++ -std=c++20 -O2 -c src/main.cpp -o main.o

# seeing the cmake build of .o
nm -C build-gcc/CMakeFiles/anatomy_cpp.dir/src/main.cpp.o
```

> Look at the`U` beside the `perf::sum_arithmetic_series` it means it is ***Undefined***, proof that the `main.cpp.o` didn't know where is the implementation until the Linker worked for it.

**4. Linking**

```bash
# create the ELF executable binary
clang++ *.o -o main
```

> Linked all the `.o`s into executable binary that are ready to run.

## Industry Notes

* Why `CMAKE_CXX_EXTENSIONS OFF` by default? Cmake using `-std:gnu++20` wich allowed special GNU/CLANG extension (such as *variable-length arrays* in stack). Turning it off is important so the `-std=c++20` is portable in all platform (including MSVC Windows).

* `[[nodiscard]]` and `noexcept`:
    * `[[nodiscard]]` (C++17) tell the compiler to give warning if the caller ignore the return value (preventing silent bug logic) 
    * `noexpect` tell the compiler that the function will not throw an exception. In *low-latency* `noexcept` make compiler erase *stack unwinding* table to produce more compact machine code.

 * Different GCC vs CLANG warning: `-Wconversion` and `-Wsign-conversion` can capture compresion of data type (example from `int64_t` to `int32_t`) in one *compiler* that are usually passed by other compiler. Running both warning can give more layer of safety.

> Notes for total beginner: You might not know all the syntax we write before which is FINE, ignore it for now, don't be too curious or you will be off the trail and procastinate, we will come back to that later in detail, for now just do what i tell u and finish the lesson.
