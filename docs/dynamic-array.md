# Dynamic Array

A **Dynamic Array** is a data structure that stores elements in contiguous memory and allows **resizing at runtime**.

Unlike static arrays in C, its size is not fixed: it uses **dynamic allocation on the heap** (manually managed memory) to adjust its capacity as needed.

---

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Resizing Strategy](#resizing-strategy)
- [Generic Design](#generic-design)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)

---

## Conceptual Structure

A Dynamic Array is typically composed of three main components:

- **data (void\*)**: pointer to the memory block where elements are stored
- **size (size_t)**: number of elements currently in use
- **capacity (size_t)**: total number of elements that can be stored without reallocation

> [!NOTE]  
> When `size == capacity`, the array must be resized.

---

## Resizing Strategy

Resizing is performed through **memory reallocation** (e.g., using `realloc`).

A common strategy is exponential growth:

- When the limit is reached, capacity is multiplied (usually by 2)

This ensures:

> [!NOTE]   
> Amortized constant-time insertion _(amortized O(1): average cost remains constant over multiple operations)_

---

## Generic Design

Since C does not support native generics, a Dynamic Array can be implemented generically using:

- `void*`: generic pointer to the data
- `element_size` (`size_t`): size in bytes of each stored element

In this model, elements are treated as raw contiguous memory, and access is performed using pointer arithmetic:

> [!NOTE]  
> element address = base + (index × element_size)

This allows storing any data type, including primitive types and user-defined structures.

---

## Responsibilities

Using Dynamic Arrays in C requires manual memory management:

- Allocation (`malloc`)
- Reallocation (`realloc`)
- Deallocation (`free`)

Improper handling may lead to:

- Memory leaks
- Invalid memory access
- Undefined behavior

---

## Complexity

|    **Operation**    |  **Complexity**  |
|:--------------------|:-----------------|
|Access               |O(1)              |
|Insertion (append)   |O(1) amortized    |
|Resizing             |O(n)              |   
