# **Generic Deque (`void*`)**

A **Generic Deque** (`AtlasDeque`) is a type-agnostic double-ended queue that stores elements of any type through dynamically allocated nodes.

A deque (*double-ended queue*) allows insertion and removal at both ends of the structure, combining the behavior of a stack and a queue in a single container.

Unlike dynamic arrays, the AtlasDS deque stores each element in an independently allocated node connected through pointers. This allows the deque to grow dynamically without requiring contiguous memory reallocation.

The implementation uses a **doubly linked structure**, where each node stores a pointer to the previous element, a pointer to the next element, and a flexible memory region containing the element data.

> [!IMPORTANT]  
> The deque module is **under active development**. Currently, only the lifecycle operations (`create` and `destroy`) are implemented. The remaining operations are planned and described in this document as such.

---

## **Table of Contents**

- [Conceptual Structure](#conceptual-structure)
- [Memory Layout](#memory-layout)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
- [Currently Implemented API](#currently-implemented-api)
    - [`atlas_deque_create()`](#atlas_deque_create)
    - [`atlas_deque_destroy()`](#atlas_deque_destroy)
- [Planned API](#planned-api)
- [Safety Guarantees](#safety-guarantees)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)
- [Usage Example](#usage-example)

---

## **Conceptual Structure**

A deque is a linear data structure that allows elements to be inserted and removed at both ends.

Elements may be inserted or removed at the **front** and at the **back**:

```
  FRONT                           BACK
    |                              |
    ▼                              ▼
+---------+    +---------+    +---------+
| Element | ⇄  | Element | ⇄  | Element |
+---------+    +---------+    +---------+
    |                              |
    ▼                              ▼
  NULL                           NULL
```

The AtlasDS implementation stores the metadata required to manage the deque independently of the stored element type.

The `AtlasDeque` structure maintains four pieces of metadata:

- `type_size` (`size_t`): size in bytes of each stored element
- `deque_size` (`size_t`): number of elements currently stored in the deque
- `front_element` (`AtlasDequeElement*`): pointer to the first element in the deque
- `back_element` (`AtlasDequeElement*`): pointer to the last element in the deque

Each `AtlasDequeElement` contains:

- `previous_element` (`AtlasDequeElement*`): pointer to the previous element in the deque
- `next_element` (`AtlasDequeElement*`): pointer to the next element in the deque
- `data` (`unsigned char[]`): flexible storage area containing the element bytes

The two link pointers allow the deque to be traversed in both directions. Keeping both `front_element` and `back_element` pointers allows insertion and removal operations to access either end directly.

The deque maintains the following structural invariants.

When the deque is empty:

```c
deque_size == 0
front_element == NULL
back_element == NULL
```

When the deque contains elements:

```c
deque_size > 0
front_element != NULL
back_element != NULL
front_element->previous_element == NULL
back_element->next_element == NULL
```

These invariants ensure that the deque always has a consistent representation of its empty and non-empty states.

---

## Memory Layout

Unlike a dynamic array, the deque does not require all elements to occupy a contiguous memory region.

Each element is stored inside an independently allocated node:

```
front_element                                         back_element
      |                                                     |
      ▼                                                     ▼
+------------------+    +------------------+    +------------------+
| previous_element |    | previous_element |    | previous_element |
|      (NULL)      | ◀──|                  | ◀──|                  |
+------------------+    +------------------+    +------------------+
|   next_element   | ──▶|   next_element   | ──▶|   next_element   |
|                  |    |                  |    |      (NULL)      |
+------------------+    +------------------+    +------------------+
|   element data   |    |   element data   |    |   element data   |
+------------------+    +------------------+    +------------------+
```

Each node contains both its linkage information and its element data.

The element storage uses a **flexible array member**:

```c
unsigned char data[];
```

This allows the node allocation to contain both the node metadata and the exact number of bytes required for the stored element.

The required allocation size for a node is conceptually:

```c
sizeof(AtlasDequeElement) + type_size
```

The deque itself stores pointers to both the first and last elements.

> [!NOTE]  
> The deque does not allocate element storage during creation. Memory for individual elements is allocated only when nodes are inserted into the deque.

---

## Current AtlasDS Implementation

The current implementation establishes the deque lifecycle and its internal memory management foundation.

Current capabilities include:

- Generic type-agnostic storage using raw bytes
- Explicit element size tracking (`type_size`)
- Dynamically allocated node-based storage
- Doubly linked node structure
- Front and back element tracking
- Empty deque initialization
- Deque size tracking
- Safe deque destruction
- Complete cleanup of all allocated nodes
- Double-pointer destruction to prevent dangling pointers
- Validation of invalid element sizes
- Validation of NULL pointers
- Allocation failure handling

> [!NOTE]  
> Insertion, removal, access, state queries, and clearing are planned but not yet implemented. Automated tests will be added once these operations are available.

---

## Currently Implemented API

The current public API consists of the following operations:

```c
AtlasDeque *atlas_deque_create(size_t type_size);

int atlas_deque_destroy(AtlasDeque **ptr_atlas_deque);
```

### `atlas_deque_create()`

Creates and initializes an empty generic deque.

The `type_size` parameter defines the number of bytes required to store each element.

The function:

- Validates that `type_size` is greater than zero
- Allocates the deque structure
- Stores the element size
- Initializes the deque size to zero
- Initializes the front element pointer to `NULL`
- Initializes the back element pointer to `NULL`

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes deque creation to fail and the function returns `NULL`.

The deque initially contains no elements.

After successful creation, the empty-state invariants are:

```c
deque_size == 0
front_element == NULL
back_element == NULL
```

The deque structure itself is allocated, but no element nodes are allocated during creation.

### `atlas_deque_destroy()`

Destroys the deque and releases all memory associated with it.

The function receives a pointer to the user's `AtlasDeque` pointer:

```c
AtlasDeque **ptr_atlas_deque
```

This allows the implementation to invalidate the caller's pointer after releasing the allocated memory.

The destruction process is:

1. Validate the pointer to the deque pointer
2. Validate the referenced deque
3. Traverse the deque from the front element
4. Store the next element before releasing the current node
5. Release every allocated node
6. Release the deque structure itself
7. Set the caller's pointer to `NULL`

The internal traversal follows the `next_element` pointer of each node until `NULL` is reached.

> [!NOTE]  
> The double-pointer interface prevents the caller from retaining a dangling pointer after successful destruction.

If either the provided pointer or the referenced deque is `NULL`, the function returns `ATLAS_ERROR_NULL`.

---

## Planned API

The following operations are planned for the deque module:

| Operation                        | Description                                  |
|:---------------------------------|:---------------------------------------------|
| `atlas_deque_push_front()`       | Inserts an element at the front              |
| `atlas_deque_push_back()`        | Inserts an element at the back               |
| `atlas_deque_pop_front()`        | Removes and returns the front element        |
| `atlas_deque_pop_back()`         | Removes and returns the back element         |
| `atlas_deque_front()`            | Reads the front element without removing it  |
| `atlas_deque_back()`             | Reads the back element without removing it   |
| `atlas_deque_size()`             | Returns the number of stored elements        |
| `atlas_deque_empty()`            | Reports whether the deque is empty           |
| `atlas_deque_clear()`            | Removes all elements, preserving the deque   |

> [!NOTE]  
> Because the deque is doubly linked, `pop_back()` is expected to run in O(1), unlike the singly linked list, where it requires traversing the structure.

---

## Safety Guarantees

Because the deque uses generic raw memory and manual dynamic allocation, the implementation performs explicit runtime validation where appropriate.

Current safety mechanisms include:

- NULL pointer validation
- Invalid element size validation
- Allocation failure handling
- Safe destruction of allocated nodes
- Complete cleanup during destruction
- Pointer invalidation after destruction

> [!NOTE]  
> These mechanisms provide defensive behavior around the deque lifecycle while preserving the low-level nature of the implementation.

---

## Responsibilities

Using a generic deque requires the caller to provide correct type information and manage the lifetime of the deque correctly.

Core responsibilities currently include:

- Providing a valid element size (`type_size`) during creation
- Ensuring `type_size` represents the actual size of the elements that will be stored
- Checking whether deque creation succeeded
- Destroying deques when they are no longer required
- Passing valid deque pointers to public operations

> [!IMPORTANT]  
> AtlasDS stores raw bytes and cannot determine whether a caller-provided pointer actually refers to a buffer large enough to hold the requested data.

Incorrect usage of generic raw-memory structures may result in:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Incorrect interpretation of stored bytes
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to demonstrate how manually managed generic data structures operate internally.

---

## Complexity

| Operation               | Complexity |
|:------------------------|:-----------|
| Creation (`create`)     | O(1)       |
| Destruction (`destroy`) | O(n)       |

> [!NOTE]  
> Deque creation performs a constant amount of work because only the deque metadata is allocated and initialized.

> [!NOTE]  
> Deque destruction traverses every allocated node to release its memory, resulting in O(n) time complexity, where `n` is the number of stored elements.

---

## Applications

Deques are commonly used as building blocks for:

- Sliding window algorithms
- Work-stealing schedulers
- Undo/redo history with bounded size
- Palindrome checking
- Breadth-first search variants (0-1 BFS)
- Buffer management
- Task scheduling

Deques are particularly useful when elements must be inserted and removed efficiently at both ends of the structure.

---

## Usage Example

The following example demonstrates the current lifecycle of the deque: creating an empty deque and destroying it.

> The API may evolve as the project is under active development.

```c
#include "atlas/deque.h"
#include "atlas/status.h"

int main(void) {
    AtlasDeque *deque = atlas_deque_create(sizeof(int));
    if (!deque) {
        return 1;
    }

    if (atlas_deque_destroy(&deque) != ATLAS_SUCCESS) {
        return 1;
    }

    if (deque != NULL) {
        return 1;
    }

    return 0;
}
```

> [!NOTE]  
> Usage examples covering insertion, removal, and access will be added as the remaining operations are implemented.