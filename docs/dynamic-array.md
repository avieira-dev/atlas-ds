# Dynamic Array

A **Dynamic Array** is a data structure that stores elements in contiguous memory and supports **runtime resizing**.

Unlike fixed-size arrays in C, Dynamic Arrays can grow dynamically through heap reallocation as more elements are inserted.

The AtlasDS implementation focuses on exposing the low-level mechanics behind dynamic storage systems, emphasizing explicit memory management and predictable runtime behavior.

---

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Resizing Strategy](#resizing-strategy)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
- [Safety Guarantees](#safety-guarantees)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)

---

## Conceptual Structure

The current AtlasDS Dynamic Array implementation is specialized for the `int` type.

The structure is composed of three main components:

- **data (`int*`)**: pointer to the contiguous memory block where elements are stored
- **size (`size_t`)**: number of elements currently stored in the array
- **capacity (`size_t`)**: total number of elements that can be stored before reallocation becomes necessary

> [!NOTE]
> When `size == capacity`, the internal buffer must be resized.

---

## Resizing Strategy

Resizing is performed through **memory reallocation** using `realloc`.

The current implementation uses an **exponential growth strategy**:

- When the array becomes full, its capacity is doubled

Example:

```text
capacity: 2 -> 4 -> 8 -> 16
```

This strategy reduces the frequency of reallocations and helps maintain efficient insertion performance.

AtlasDS also provides explicit capacity management via `reserve()`, enabling manual preallocation of memory in addition to the automatic growth strategy used by `push()`.

> [!NOTE]
> Dynamic Arrays provide amortized constant-time insertion.  
> Amortized O(1) means that although some insertions trigger expensive reallocations, the average insertion cost remains constant over multiple operations.

---

## Current AtlasDS Implementation

The current implementation focuses on fundamental low-level concepts, including:

- Manual memory allocation and deallocation
- Dynamic resizing using exponential growth
- Contiguous memory storage
- Safe lifecycle management (`create` / `destroy`)
- Element insertion using append semantics
- Indexed element mutation using `set`
- Element removal using stack-like semantics (`pop`)
- Bounds-checked indexed access
- Runtime size and capacity tracking
- Manual capacity control via `reserve`
- Logical reset operations using `clear`
- Empty-state queries using `empty`
- Buffer reuse without reallocation
- Defensive NULL validation
- Automated runtime testing using CMake

### Currently Implemented API

```c
AtlasArray *atlas_array_create(size_t initial_capacity);

void atlas_array_destroy(AtlasArray **ptr_atlas_array);

int atlas_array_push(AtlasArray *arr, int value);

int atlas_array_pop(AtlasArray *arr, int *out_value);

int atlas_array_get(const AtlasArray *arr, size_t index, int *out_value);

int atlas_array_set(AtlasArray *arr, size_t index, int new_value);

size_t atlas_array_size(const AtlasArray *arr);

size_t atlas_array_capacity(const AtlasArray *arr);

int atlas_array_reserve(AtlasArray *arr, size_t new_capacity);

int atlas_array_clear(AtlasArray *arr);

int atlas_array_empty(const AtlasArray *arr, bool *empty);
```

> [!IMPORTANT]
> The current implementation supports only the `int` type.

> [!NOTE]
> Generic support using `void*` and element size abstraction is planned for future versions of AtlasDS.

---

## Safety Guarantees

The current implementation includes defensive runtime validation to reduce invalid memory access and improve API predictability.

Implemented safety mechanisms include:

- Bounds checking during indexed access
- NULL pointer validation
- Safe destruction using double pointers
- Reallocation failure handling
- Underflow protection during removal operations
- Explicit ownership management

> [!NOTE]
> These checks are designed to improve stability and predictability during low-level memory manipulation.

---

## Responsibilities

Using Dynamic Arrays in C requires explicit memory management.

Core responsibilities include:

- Allocation (`malloc`)
- Reallocation (`realloc`)
- Deallocation (`free`)

Improper handling may lead to:

- Memory leaks
- Invalid memory access
- Undefined behavior
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to help developers better understand low-level systems behavior.

---

## Complexity

| **Operation**       | **Complexity**  |
|:--------------------|:----------------|
| Access (`get`)      | O(1)            |
| Mutation (`set`)    | O(1)            |
| Empty (`empty`)     | O(1)            |
| Insertion (`push`)  | O(1) amortized  |
| Removal (`pop`)     | O(1)            |
| Clear (`clear`)     | O(1)            |
| Reserve (`reserve`) | O(n) worst-case |
| Resizing            | O(n)            |
| Size query          | O(1)            |
| Capacity query      | O(1)            |

---

## Applications

Dynamic Arrays are commonly used in:

- Game engines
- Entity systems
- Rendering pipelines
- Runtime buffers
- ECS storage systems
- Systems programming
- Custom allocators
- General-purpose collections

Dynamic Arrays are often one of the foundational structures behind higher-level containers and runtime systems.