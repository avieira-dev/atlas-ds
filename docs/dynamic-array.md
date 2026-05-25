# Dynamic Array

A **Dynamic Array** is a data structure that stores elements in contiguous memory and allows **resizing at runtime**.

Unlike static arrays in C, its size is not fixed: it uses **dynamic allocation on the heap** (manually managed memory) to adjust its capacity as needed.

---

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Resizing Strategy](#resizing-strategy)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
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

A common strategy for Dynamic Arrays is exponential growth:

- When the array becomes full, its capacity is increased (typically doubled)

This strategy helps reduce the frequency of reallocations and provides:

> [!NOTE]  
> Amortized constant-time insertion _(amortized O(1): average insertion cost remains constant over multiple operations)_

---

## Current AtlasDS Implementation

The current implementation focuses on fundamental low-level concepts, including:

- Manual memory allocation and reallocation
- Dynamic resizing using exponential growth
- Safe lifecycle management (create / destroy)
- Element insertion via push operation

> [!IMPORTANT]  
> The current implementation only supports the `int` type.

> [!NOTE]  
> Generic support using `void*` and element size abstraction is planned for future versions of AtlasDS.

---

## Responsibilities

Using Dynamic Arrays in C requires explicit memory management:

- Allocation (`malloc`)
- Reallocation (`realloc`)
- Deallocation (`free`)

Improper memory handling may lead to:

- Memory leaks
- Invalid memory access
- Undefined behavior

---

## Complexity

| **Operation**      | **Complexity** |
|:-------------------|:---------------|
| Access             | O(1)           |
| Insertion (append) | O(1) amortized |
| Resizing           | O(n)           |

---

## Applications

Dynamic Arrays are commonly used in:

- Game engines
- Entity systems
- Rendering pipelines
- Runtime buffers
- Systems programming
- General-purpose collections