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

* **Scope**: An area inside our code (with syntax `{}`) that control variable/value to exist or not.

* **Lifetime**: A Timeline when the value/object is created in memory until it is destroyed or memory being freed (it is **not** guaranteed ends with only `{}`, for example pointer, we'll get there later).

## Memory Map: Stack vs Heap
