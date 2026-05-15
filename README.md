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
- [Quick Example](#quick-example)
- [Planned Structures](#planned-structures)
- [Repository Structure](#repository-structure)
- [Build](#build)
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

- **From scratch** — no stdlib containers, no shortcuts
- **Explicit memory control** — allocation and deallocation are always visible
- **Systems-oriented patterns** — implementations reflect real usage in low-level programming

---

## Quick Example

> The API may evolve as the project is under active development.

```c
#include <atlas/array.h>

int main() {
    atlas_array* arr = atlas_array_create(sizeof(int));

    int value = 10;
    atlas_array_push(arr, &value);

    int* data = (int*)atlas_array_get(arr, 0);

    atlas_array_destroy(arr);
    return 0;
}
```

---

## Planned Structures

Each module will include an implementation, usage examples, documentation, and automated tests.

| Structure | Status |
|---|---|
| Dynamic Arrays | 🔲 Planned |
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
│   └── atlas/          # Public headers
├── src/                # Implementations
├── examples/           # Usage examples per structure
├── tests/              # Automated tests
├── docs/               # Documentation
├── build/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Integration

Clone AtlasDS into your project directory and add it via CMake:

```bash
git clone https://github.com/avieira-dev/atlas-ds.git
```

```cmake
add_subdirectory(atlas-ds)
target_link_libraries(your_project atlas)
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
> This is the recommended integration method. Additional methods may be supported in the future.

---

## Developer

**Alexandre Vieira**
GitHub: [@avieira-dev](https://github.com/avieira-dev)

---

## License

Distributed under the [MIT License](LICENSE). See `LICENSE` for details.