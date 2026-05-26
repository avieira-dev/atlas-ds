<div align="center">

<h1>AtlasDS</h1>

<p>A low-level, memory-oriented data structures library in pure C, designed to expose how data structures truly work under the hood.</p>

<p>
  <img src="https://img.shields.io/badge/status-in%20development-f39c12?style=flat-square"/>
  <img src="https://img.shields.io/badge/language-C-A8B9CC?style=flat-square&logo=c&logoColor=white"/>
  <img src="https://img.shields.io/badge/build-CMake-8a63d2?style=flat-square&logo=cmake&logoColor=white"/>
  <img src="https://img.shields.io/badge/license-MIT-6e7781?style=flat-square"/>
</p>

</div>

---

> [!NOTE]
> New data structures will be implemented progressively as the atlas grows.

## Table of Contents

- [Overview](#overview)
- [Philosophy](#philosophy)
- [Current Features](#current-features)
- [Quick Example](#quick-example)
- [Planned Structures](#planned-structures)
- [Repository Structure](#repository-structure)
- [Build](#build)
- [Running Tests](#running-tests)
- [Integration](#integration)
- [Developer](#developer)
- [License](#license)

---

## Overview

AtlasDS explores how **fundamental data structures** behave from a low-level systems perspective — exposing their internal mechanics such as memory layout, pointer relationships, and structural invariants.

Rather than providing high-level abstractions, every implementation is transparent by design, treating each structure not as a container, but as a mechanical system of memory.

The library also serves as a reference implementation for students and developers studying computer science fundamentals.

**Naming convention:** all public symbols use the `atlas_` prefix to ensure clarity and avoid collisions in C translation units.

---

## Philosophy

- **From scratch** — fully manual implementations, no shortcuts
- **Explicit memory control** — allocation and deallocation are always visible
- **Systems-oriented patterns** — implementations reflect real usage in low-level programming
- **Defensive programming** — APIs validate invalid states and unsafe access patterns
- **Transparent memory behavior** — resizing and allocation behavior remain observable

---

## Current Features

### Dynamic Array (`int` implementation)

Current capabilities:

- Dynamic allocation and destruction
- Automatic runtime resizing
- Contiguous memory storage
- Bounds-checked element access
- Logical element removal
- Capacity tracking
- Size tracking
- Safe NULL handling
- Stack-like push/pop behavior
- Automated tests using CMake

Implemented public API:

```c
AtlasArray *atlas_array_create(size_t initial_capacity);

void atlas_array_destroy(AtlasArray **ptr_atlas_array);

int atlas_array_push(AtlasArray *arr, int value);

int atlas_array_pop(AtlasArray *arr, int *out_value);

int atlas_array_get(const AtlasArray *arr, size_t index, int *out_value);

size_t atlas_array_size(const AtlasArray *arr);

size_t atlas_array_capacity(const AtlasArray *arr);
```

> [!IMPORTANT]
> The current implementation supports only the `int` type.

> [!NOTE]
> Generic support using `void*` and element size tracking is planned for future versions.

---

## Quick Example

> The API may evolve as the project is under active development.

```c
#include <atlas/array.h>

int main(void) {

    AtlasArray *arr = atlas_array_create(2);

    if (!arr) {
        return 1;
    }

    atlas_array_push(arr, 10);
    atlas_array_push(arr, 20);
    atlas_array_push(arr, 30); // triggers automatic resize

    size_t size = atlas_array_size(arr);
    size_t capacity = atlas_array_capacity(arr);

    int retrieved_value = 0;

    if (atlas_array_get(arr, 1, &retrieved_value) != 0) {
        atlas_array_destroy(&arr);
        return 1;
    }

    int removed_value = 0;

    if (atlas_array_pop(arr, &removed_value) != 0) {
        atlas_array_destroy(&arr);
        return 1;
    }

    atlas_array_destroy(&arr);

    return 0;
}
```

---

## Planned Structures

Each module will include an implementation, usage examples, documentation, and automated tests.

| Structure | Status |
|---|---|
| Dynamic Array | 🚧 In Progress |
| Linked Lists | 🔲 Planned |
| Stacks | 🔲 Planned |
| Queues | 🔲 Planned |
| Hash Tables | 🔲 Planned |
| Binary Search Trees | 🔲 Planned |
| Heaps / Priority Queues | 🔲 Planned |
| Graph Representations | 🔲 Planned |

---

## Repository Structure

```text
atlas-ds/
├── include/
│   └── atlas/
├── src/
├── examples/
├── tests/
├── docs/
├── build/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Build

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## Running Tests

After building the project:

```bash
./tests/test_array
```

Example output:

```text
[INFO] Starting AtlasDS dynamic array tests...
[OK] Dynamic array created successfully.
[OK] Automatic resizing completed successfully.
[OK] Bounds checking validated successfully.
[OK] Metadata queries validated successfully.
[OK] Pop operation validated successfully.
[INFO] All tests completed successfully.
```

---

## Integration

Clone AtlasDS into your project directory and add it via CMake:

```bash
git clone https://github.com/avieira-dev/atlas-ds.git
```

```cmake
add_subdirectory(atlas-ds)
target_link_libraries(your_project atlasds)
```

Expected project layout after integration:

```text
your-project/
├── atlas-ds/
├── src/
├── CMakeLists.txt
└── README.md
```

> [!NOTE]
> This is the recommended integration method. Additional integration methods may be supported in future releases.

---

## Developer

**Alexandre Vieira**  
GitHub: [@avieira-dev](https://github.com/avieira-dev)

---

## License

Distributed under the [MIT License](LICENSE). See `LICENSE` for details.