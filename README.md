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
- [Documentation](#documentation)
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

- Integer-specialized implementation
- Dynamic allocation and destruction
- Automatic runtime resizing
- Contiguous memory storage
- Indexed element access (`get`) and mutation (`set`)
- Stack-like insertion (`push`) and removal (`pop`)
- Indexed insertion (`insert`) and removal (`erase`)
- Element swapping (`swap`)
- Array copying (`copy`) and deep cloning (`clone`)
- Value lookup (`find`) and membership queries (`contains`)
- Size, capacity, and empty-state queries
- First (`front`) and last (`back`) element access
- Manual capacity management (`reserve`, `shrink_to_fit`, `clear`)
- Defensive validation of pointers and invalid states
- Prevention of dangling pointers via double-pointer destruction
- Automated tests covering all implemented public APIs

See the full API reference and usage example in [`docs/dynamic-array.md`](docs/dynamic-array.md).

### Generic Dynamic Array (`void*` implementation)

Current capabilities:

- Generic type-agnostic storage via `void*`
- Element size tracking in bytes (`type_size`)
- Dynamic allocation and destruction
- Automatic runtime resizing
- Contiguous raw memory storage
- Size, capacity, and empty-state queries
- First (`front`) and last (`back`) element access
- Indexed element access (`get`) and mutation (`set`)
- Stack-like insertion (`push`) and removal (`pop`)
- Indexed insertion (`insert`) and removal (`erase`)
- Indexed element swapping (`swap`)
- Array copying (`copy`) and deep cloning (`clone`)
- Value lookup (`find`) and membership queries (`contains`) via user-provided comparison
- Defensive validation of pointers and initialization states
- Prevention of dangling pointers via double-pointer destruction
- Automated tests covering all implemented public APIs

> [!IMPORTANT]  
> The generic implementation stores raw bytes and does not perform any type checking. The caller is responsible for providing the correct element size, data type, and comparison logic when using search operations.

See the full API reference and usage example in [`docs/dynamic-array-void.md`](docs/dynamic-array-void.md).

### Generic Linked List (`void*` implementation)

Current capabilities:

- Generic type-agnostic storage using flexible node-based allocation
- Element size tracking in bytes (`type_size`)
- Dynamic node allocation and destruction
- Singly linked node structure with first and last node tracking
- Empty-state initialization and validation
- Size and empty-state queries (`size`, `empty`)
- First (`front`) and last (`back`) element access
- Indexed element access (`get`) and mutation (`set`)
- Indexed element swapping (`swap`)
- Insertion at the beginning (`push_front`)
- Insertion at the end (`push_back`)
- Indexed insertion (`insert`)
- Removal from the beginning (`pop_front`)
- Removal from the end (`pop_back`)
- Indexed removal (`erase`)
- List clearing while preserving the list structure (`clear`)
- Safe destruction of all allocated nodes
- Prevention of dangling pointers via double-pointer destruction
- Defensive validation of pointers, indices, and empty-list operations

> [!NOTE]  
> The linked list implementation is currently under active development. Additional operations such as searching, copying, cloning, reversing, and advanced traversal utilities will be added progressively.

See the full API reference and usage example in [`docs/list.md`](docs/list.md).

---

## Documentation

Detailed documentation for each structure — including conceptual design, memory layout, complexity analysis, safety guarantees, and usage examples — is available in [`docs/`](docs/):

- [Dynamic Array](docs/dynamic-array.md)
- [Generic Dynamic Array](docs/dynamic-array-void.md)
- [Generic Linked List](docs/list.md)

---

## Planned Structures

Each module will include an implementation, usage examples, documentation, and automated tests.

| Structure               | Status         |
|-------------------------|----------------|
| Dynamic Array (int)     | ✅ Complete    |
| Dynamic Array (void*)   | ✅ Complete    |
| Linked Lists            | 🚧 In Progress |
| Stacks                  | 🔲 Planned     |
| Queues                  | 🔲 Planned     |
| Deque                   | 🔲 Planned     |
| Binary Search Trees     | 🔲 Planned     |
| Heaps / Priority Queues | 🔲 Planned     |
| Hash Tables             | 🔲 Planned     |
| Graph Representations   | 🔲 Planned     |

---

## Repository Structure

```text
atlas-ds/
├── include/
│   └── atlas/
├── src/
├── tests/
├── docs/
├── build/
├── .gitignore
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
./tests/test_array_void
./tests/test_list
```

## Example output

### Generic Dynamic Array (`void*` implementation)

```text
=========================================================
          AtlasDS - Generic Dynamic Array Tests
=========================================================

[INFO] Starting AtlasDS generic dynamic array tests...

[INFO] Running capacity management tests...
[OK] Reserve test passed.
[OK] Reserve validation passed.
[OK] Clear test passed.
[OK] Clear validation passed.
[OK] Shrink-to-fit test passed.
[OK] Shrink-to-fit validation passed.

[SUCCESS] All Generic Dynamic Array tests passed successfully!
```

### Generic Linked List (`void*` implementation)

```text
========================================================
		          AtlasDS - List Tests
========================================================

[INFO] Starting AtlasDS list tests...

[INFO] Running removal tests...
[OK] Pop front operation passed.
[OK] Pop back operation passed.
[OK] Pop on empty list validation passed.
[OK] Front/Back on empty list validation passed.
[OK] Erase at beginning operation passed.
[OK] Erase middle operation passed.
[OK] Erase end operation passed.
[OK] Erase invalid index validation passed.

[SUCCESS] All tests completed successfully.
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
GitHub: **[@avieira-dev](https://github.com/avieira-dev)**

---

## License

Distributed under the [MIT License](LICENSE). See `LICENSE` for details.