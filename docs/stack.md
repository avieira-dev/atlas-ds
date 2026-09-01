# Generic Stack (`Void*`)

A **Generic Stack** (`AtlasStack`) is a type-agnostic stack data structure that stores elements of any type through dynamically allocated nodes.

A stack follows the **LIFO (Last In, First Out)** principle, meaning that the last element inserted into the structure is the first element removed.

Unlike dynamic arrays, the current AtlasDS stack implementation stores each element in an independently allocated node connected through pointers. This allows the stack to grow dynamically without requiring contiguous memory reallocation.

The current implementation uses a **singly linked structure**, where each node stores a pointer to the previous element and a flexible memory region containing the element data.

---

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Memory Layout](#memory-layout)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
- [Currently Implemented API](#currently-implemented-api)
- [Safety Guarantees](#safety-guarantees)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)
- [Usage Example](#usage-example)

---

## Conceptual Structure

A stack is a linear data structure organized according to the **LIFO (Last In, First Out)** principle.

Elements are inserted and removed from the same logical end of the structure, known as the **top**.

Conceptually:

```text
        TOP
         |
         ▼
    +---------+
    | Element |
    +---------+
         |
         ▼
    +---------+
    | Element |
    +---------+
         |
         ▼
    +---------+
    | Element |
    +---------+
         |
         ▼
        NULL
```

The AtlasDS implementation stores the metadata required to manage the stack independently of the stored element type.

The `AtlasStack` structure maintains three pieces of metadata:

- `type_size` (`size_t`): size in bytes of each stored element
- `stack_size` (`size_t`): number of elements currently stored in the stack
- `top_element` (`AtlasStackElement*`): pointer to the element currently at the top of the stack

Each `AtlasStackElement` contains:

- `previous_element` (`AtlasStackElement*`): pointer to the element below the current node
- `data` (`unsigned char[]`): flexible storage area containing the element bytes

The `previous_element` relationship allows the implementation to traverse the stack from the top toward the bottom.

---

## Memory Layout

Unlike a dynamic array, the stack does not require all elements to occupy a contiguous memory region.

Each element is stored inside an independently allocated node:

```text
   top_element
        |
        ▼
+------------------+
| previous_element | -------+
+------------------+        │
|   element data   |        │
+------------------+        |
                            ▼
                 +------------------+
                 | previous_element | ------+
                 +------------------+       |
                 |   element data   |       |
                 +------------------+       |
                                            ▼
                                  +------------------+
                                  | previous_element | ----▶ NULL
                                  +------------------+
                                  |   element data   |
                                  +------------------+
```

Each node contains both its linkage information and its element data.

The element storage uses a **flexible array member**:

```c
unsigned char data[];
```

This allows the node allocation to contain both the node metadata and the exact number of bytes required for the stored element.

The required allocation size for a node is conceptually:

```c
sizeof(AtlasStackElement) + type_size
```

The stack itself only stores a pointer to the top element. Traversal toward the bottom is performed by following each node's `previous_element` pointer.

> [!NOTE]  
> The stack does not allocate element storage during creation. Memory for individual elements is allocated only when nodes are inserted into the stack.

---

## Current AtlasDS Implementation

The current implementation provides the initial lifecycle required to create and safely destroy a generic stack.

Current capabilities include:

- Generic type-agnostic storage design using raw bytes
- Explicit element size tracking (type_size)
- Dynamically allocated node-based storage
- Singly linked node structure
- Top element tracking
- Empty stack initialization
- Stack size tracking
- Safe stack destruction
- Complete cleanup of all allocated nodes
- Double-pointer destruction to prevent dangling pointers
- Validation of invalid element sizes
- Validation of NULL pointers during destruction
- Automated lifecycle tests

The stack currently represents the foundation for the LIFO operations that will be implemented as the data structure evolves.

> [!NOTE]  
> Stack insertion and removal operations are not yet part of the public API. The current implementation focuses on establishing the stack's internal representation and lifecycle management.

---

## Currently Implemented API

The current public API consists of the stack lifecycle operations:

```c
AtlasStack *atlas_stack_create(size_t type_size);

int atlas_stack_destroy(AtlasStack **ptr_atlas_stack);
```

`atlas_stack_create()`

Creates and initializes an empty generic stack.

The `type_size` parameter defines the number of bytes required to store each element.

The function:

- Validates that type_size is greater than zero.
- Allocates the stack structure.
- Stores the element size.
- Initializes the stack size to zero.
- Initializes the top element pointer to NULL.

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes stack creation to fail and the function returns `NULL`.

The stack initially contains no elements.

`atlas_stack_destroy()`

Destroys the stack and releases all memory associated with it.

The function receives a pointer to the user's `AtlasStack` pointer:

```C
AtlasStack **ptr_atlas_stack
```

This allows the implementation to invalidate the caller's pointer after releasing the allocated memory.

The destruction process is:

- Validate the pointer to the stack pointer.
- Validate the referenced stack.
- Traverse the stack from the top element.
- Store the previous element before releasing the current node.
- Release every allocated node.
- Release the stack structure itself.
- Set the caller's pointer to `NULL`.

> [!NOTE]  
> The double-pointer interface prevents the caller from retaining a dangling pointer after successful destruction.

If either the provided pointer or the referenced stack is `NULL`, the function returns `ATLAS_ERROR_NULL`.

---

## Safety Guarantees

Because the stack uses generic raw memory and manual dynamic allocation, the implementation performs explicit runtime validation where appropriate.

Current safety mechanisms include:

- NULL pointer validation
- Invalid element size validation
- Allocation failure handling
- Safe destruction of allocated nodes
- Complete cleanup during destruction
- Pointer invalidation after destruction

> [!NOTE]  
> These mechanisms provide defensive behavior around the stack lifecycle while preserving the low-level nature of the implementation.

---

## Responsibilities

Using a generic stack requires the caller to provide correct type information and manage the lifetime of the stack correctly.

Core responsibilities currently include:

- Providing a valid element size (`type_size`) during creation
- Ensuring `type_size` represents the actual size of the elements that will be stored
- Checking whether stack creation succeeded
- Destroying stacks when they are no longer required
- Passing valid stack pointers to public operations

As generic stack operations are introduced, additional responsibilities will include providing correctly sized values and valid output buffers.

Incorrect usage of generic raw-memory structures may result in:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Incorrect interpretation of stored bytes
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to demonstrate how manually managed generic data structures operate internally.

---

## Complexity

| Operation              | Complexity |
|:-----------------------|:-----------|
| Creation(`create`)     | O(1)       |
| Destruction(`destroy`) | O(n)       |

> [!NOTE]  
> Stack creation performs a constant amount of work because only the stack metadata is allocated and initialized.

> [!NOTE]  
> Stack destruction traverses every allocated node to release its memory, resulting in O(n) time complexity, where `n` is the number of stored elements.

> [!NOTE]  
> The current implementation does not yet expose insertion or removal operations. Once the LIFO operations are implemented, their complexity will be documented here.

---

## Applications

Stacks are commonly used as building blocks for:

- Function call management
- Expression evaluation
- Parsing algorithms
- Depth-first search
- Backtracking algorithms
- Undo/redo systems
- Command processing
- Memory and execution models
- Compiler and interpreter implementations

Stacks are particularly useful when elements must be processed in reverse order of insertion.

The LIFO model makes stacks a fundamental abstraction in computer science and systems programming.

---

## Usage Example

> [!NOTE]  
> The stack API is currently limited to creation and destruction. Element insertion and removal operations will be added as the implementation evolves.