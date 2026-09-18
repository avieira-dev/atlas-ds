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

## Overview

AtlasDS explores how **fundamental data structures** behave from a low-level systems perspective — exposing their internal mechanics such as memory layout, pointer relationships, and structural invariants.

Rather than providing high-level abstractions, every implementation is transparent by design, treating each structure not as a container, but as a mechanical system of memory.

The library also serves as a reference implementation for students and developers studying computer science fundamentals.

**Naming convention:** all public symbols use the `atlas_` prefix to ensure clarity and avoid collisions in C translation units.

> [!NOTE]  
> New data structures will be implemented progressively as the atlas grows.

---

## Table of Contents

- [Philosophy](#philosophy)
- [Current Features](#current-features)
  - [Dynamic Array](#dynamic-array-int-implementation)
  - [Generic Dynamic Array](#generic-dynamic-array-void-implementation)
  - [Generic Linked List](#generic-linked-list-void-implementation)
  - [Generic Stack](#generic-stack-void-implementation)
- [Documentation](#documentation)
- [Planned Structures](#planned-structures)
- [Repository Structure](#repository-structure)
- [Build](#build)
- [Running Tests](#running-tests)
  - [Example Output](#example-output)
    - [Generic Dynamic Array](#generic-dynamic-array-void-implementation-1)
    - [Generic Linked List](#generic-linked-list-void-implementation-1)
    - [Generic Stack](#generic-stack-void-implementation-1)
- [Integration](#integration)
- [Author](#author)
- [License](#license)

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
- Value search (`find`)
- Membership queries (`contains`)
- Indexed element swapping (`swap`)
- Insertion at the beginning (`push_front`)
- Insertion at the end (`push_back`)
- Indexed insertion (`insert`)
- Removal from the beginning (`pop_front`)
- Removal from the end (`pop_back`)
- Indexed removal (`erase`)
- List clearing while preserving the list structure (`clear`)
- List copying (`copy`)
- Deep list cloning (`clone`)
- List reversal (`reverse`)
- Safe destruction of all allocated nodes
- Prevention of dangling pointers via double-pointer destruction
- Defensive validation of pointers, indices, and empty-list operations
- Automated tests covering all implemented public APIs

See the full API reference and usage example in [`docs/list.md`](docs/list.md).

### Generic Stack (`void*` implementation)

Current capabilities:

- Generic type-agnostic storage using dynamically allocated nodes
- Element size tracking in bytes (`type_size`)
- Singly linked node structure with top element tracking
- Dynamic element allocation and destruction
- Stack size tracking
- Last-in, first-out (LIFO) element organization
- Stack creation and destruction
- Element insertion via `push`
- Element removal via `pop`
- Top element access via `top`
- Safe traversal and release of all allocated elements
- Defensive validation of type size, pointers, and initialization states
- Prevention of dangling pointers via double-pointer destruction
- Automated lifecycle, insertion, removal, and access tests

See the full API reference and usage example in [`docs/stack.md`](docs/stack.md).

---

## Documentation

Detailed documentation for each structure — including conceptual design, memory layout, complexity analysis, safety guarantees, and usage examples — is available in [`docs/`](docs/):

- [Dynamic Array](docs/dynamic-array.md)
- [Generic Dynamic Array](docs/dynamic-array-void.md)
- [Generic Linked List](docs/list.md)
- [Generic Stack](docs/stack.md)

---

## Planned Structures

Each module will include an implementation, usage examples, documentation, and automated tests.

| Structure               | Status                      |
|-------------------------|-----------------------------|
| Dynamic Array (int)     | ████████████████████ `100%` |
| Dynamic Array (void*)   | ████████████████████ `100%` |
| Linked Lists            | ████████████████████ `100%` |
| Stacks                  | ██████████░░░░░░░░░░ `50%`  |
| Queues                  | ░░░░░░░░░░░░░░░░░░░░ `0%`   |
| Deque                   | ░░░░░░░░░░░░░░░░░░░░ `0%`   |
| Binary Search Trees     | ░░░░░░░░░░░░░░░░░░░░ `0%`   |
| Heaps / Priority Queues | ░░░░░░░░░░░░░░░░░░░░ `0%`   |
| Hash Tables             | ░░░░░░░░░░░░░░░░░░░░ `0%`   |
| Graph Representations   | ░░░░░░░░░░░░░░░░░░░░ `0%`   |

---

## Repository Structure

```text
atlas-ds/
├── build/
├── docs/
│   ├── dynamic-array-void.md
│   ├── dynamic-array.md
│   ├── index.md
│   ├── list.md
│   └── stack.md
├── examples/
├── include/
│   └── atlas/
|       ├── array_void.h
|       ├── array.h
|       ├── list.h
|       ├── stack.h
|       ├── status.h
│       └── terminal.h
├── src/
│   ├── array_void.c
│   ├── array.c
│   ├── list.c
│   └── stack.c
├── tests/
│   ├── CMakeLists.txt
│   ├── test_array_void.c
│   ├── test_array.c
│   ├── test_list.c
│   └── test_stack.c
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
./tests/test_stack
```

### Example output

#### Generic Dynamic Array (`void*` implementation)

```text
╭────────────────────────────────────────────────────────╮
│          AtlasDS - Generic Dynamic Array Tests         │
╰────────────────────────────────────────────────────────╯

ℹ Starting AtlasDS generic dynamic array tests...

➤ Lifecycle
────────────────────────────────────────────────────────
  ✔ Create/Destroy operation
  ✔ Type size validation
  ✔ NULL destroy validation

➤ Insertion
────────────────────────────────────────────────────────
  ✔ Push/pop with integers
  ✔ Push/pop with doubles
  ✔ NULL push validation
  ✔ Insert in middle
  ✔ NULL/bounds insert validation

➤ Removal
────────────────────────────────────────────────────────
  ✔ Pop from empty array
  ✔ NULL pop validation
  ✔ Erase in middle
  ✔ NULL/bounds erase validation

➤ Access
────────────────────────────────────────────────────────
  ✔ Get/Set element
  ✔ NULL/bounds get validation
  ✔ NULL/bounds set validation
  ✔ Front/Back element retrieval
  ✔ Front/Back on empty array
  ✔ NULL Front/Back validation

➤ Search
────────────────────────────────────────────────────────
  ✔ Find value with comparator
  ✔ NULL find validation
  ✔ Contains value with comparator
  ✔ NULL contains validation

➤ Capacity
────────────────────────────────────────────────────────
  ✔ Reserve expands capacity
  ✔ NULL/downward reserve validation
  ✔ Shrink to fit
  ✔ NULL shrink_to_fit validation

➤ Utility
────────────────────────────────────────────────────────
  ✔ Element exchange
  ✔ NULL/bounds swap validation
  ✔ Array copy
  ✔ NULL/type-mismatch copy validation
  ✔ Clone array
  ✔ NULL clone validation

➤ Metadata & Clear
────────────────────────────────────────────────────────
  ✔ Size tracking
  ✔ Capacity tracking
  ✔ Empty state detection
  ✔ Clear without truncating capacity
  ✔ NULL clear validation
  ✔ NULL metadata validation

════════════════════════════════════════════════════════

 ✔ SUCCESS: All tests were completed successfully.

```

#### Generic Linked List (`void*` implementation)

```text
╭────────────────────────────────────────────────────────╮
│                  AtlasDS - List Tests                  │
╰────────────────────────────────────────────────────────╯

ℹ Starting AtlasDS list tests...

➤ Lifecycle
────────────────────────────────────────────────────────
  ✔ Create/Destroy operation
  ✔ Type size validation
  ✔ NULL destroy validation

➤ Insertion
────────────────────────────────────────────────────────
  ✔ Push front operation
  ✔ Push back operation
  ✔ Insert at beginning operation
  ✔ Insert middle operation
  ✔ Insert end operation
  ✔ Insert invalid index validation

➤ Access
────────────────────────────────────────────────────────
  ✔ Set operation
  ✔ Empty check
  ✔ Get/Set bounds validation

➤ Search
────────────────────────────────────────────────────────
  ✔ Find operation
  ✔ Find not found validation
  ✔ Find on empty list validation
  ✔ Find NULL validation
  ✔ Contains operation
  ✔ Contains not found validation
  ✔ Contains on empty list validation
  ✔ Contains NULL validation

➤ Swap
────────────────────────────────────────────────────────
  ✔ Swap operation
  ✔ Swap same index validation
  ✔ Swap invalid index validation
  ✔ Swap on single-element list
  ✔ Swap on empty list validation

➤ Reverse
────────────────────────────────────────────────────────
  ✔ Reverse operation
  ✔ Reverse single-element list
  ✔ Reverse on empty list validation
  ✔ Reverse NULL validation

➤ Removal
────────────────────────────────────────────────────────
  ✔ Pop front operation
  ✔ Pop back operation
  ✔ Pop on empty list validation
  ✔ Front/Back on empty list validation
  ✔ Erase at beginning operation
  ✔ Erase middle operation
  ✔ Erase end operation
  ✔ Erase invalid index validation

➤ Clear
────────────────────────────────────────────────────────
  ✔ Clear operation on empty list
  ✔ Clear operation on single-element list
  ✔ Clear operation on multi-element list
  ✔ List reuse after clear
  ✔ Clear NULL validation

➤ Copy and Clone
────────────────────────────────────────────────────────
  ✔ Copy operation
  ✔ Copy independence validation
  ✔ Clone operation
  ✔ Clone independence validation

════════════════════════════════════════════════════════

 ✔ SUCCESS: All tests were completed successfully.

```

#### Generic Stack (`void*` implementation)

```text
╭────────────────────────────────────────────────────────╮
│                 AtlasDS - Stack Tests                  │
╰────────────────────────────────────────────────────────╯

ℹ Starting AtlasDS stack tests...

➤ Lifecycle
────────────────────────────────────────────────────────
  ✔ Create/Destroy operation
  ✔ Type size validation
  ✔ NULL destroy validation

➤ Insertion
────────────────────────────────────────────────────────
  ✔ Push into empty stack
  ✔ Push multiple elements
  ✔ NULL push validation

➤ Removal
────────────────────────────────────────────────────────
  ✔ Pop single element
  ✔ Pop multiple elements
  ✔ Pop from empty stack
  ✔ NULL pop validation

➤ Access
────────────────────────────────────────────────────────
  ✔ Top single element
  ✔ Top multiple elements
  ✔ Top preserves element
  ✔ Top from empty stack
  ✔ NULL top validation

════════════════════════════════════════════════════════

 ✔ SUCCESS: All tests were completed successfully.
 
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

## Author

**Alexandre Vieira**  
GitHub: **[@avieira-dev](https://github.com/avieira-dev)**

---

## License

Distributed under the [MIT License](LICENSE). See `LICENSE` for details.