# Generic Queue (void*)

A **Generic Queue** (`AtlasQueue`) is a type-agnostic queue data structure that stores elements of any type through dynamically allocated nodes.

A queue follows the **FIFO (First In, First Out)** principle, meaning that the first element inserted into the structure is the first element removed.

Unlike dynamic arrays, the current AtlasDS queue implementation stores each element in an independently allocated node connected through pointers. This allows the queue to grow dynamically without requiring contiguous memory reallocation.

The current implementation uses a **singly linked structure**, where each node stores a pointer to the next element and a flexible memory region containing the element data.

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Memory Layout](#memory-layout)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
- [Currently Implemented API](#currently-implemented-api)
  - [`atlas_queue_create()`](#atlas_queue_create)
  - [`atlas_queue_destroy()`](#atlas_queue_destroy)
- [Safety Guarantees](#safety-guarantees)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)

---

## Conceptual Structure

A queue is a linear data structure organized according to the FIFO (First In, First Out) principle.

Elements are inserted at one end of the structure, known as the **back**, and removed from the opposite end, known as the **front**.

Conceptually:

```
         FRONT                           BACK
           |                              |
           ▼                              ▼
      +---------+    +---------+    +---------+
      | Element | —▶ | Element | —▶ | Element | —▶ NULL
      +---------+    +---------+    +---------+
```

The AtlasDS implementation stores the metadata required to manage the queue independently of the stored element type.

The `AtlasQueue` structure maintains four pieces of metadata:

- `type_size` (`size_t`): size in bytes of each stored element
- `queue_size` (`size_t`): number of elements currently stored in the queue
- `front_element` (`AtlasQueueElement*`): pointer to the first element in the queue
- `back_element` (`AtlasQueueElement*`): pointer to the last element in the queue

Each `AtlasQueueElement` contains:

- `next_element` (`AtlasQueueElement*`): pointer to the next element in the queue
- `data` (`unsigned char[]`): flexible storage area containing the element bytes

The `next_element` relationship allows the implementation to traverse the queue from the front toward the back.

Keeping both `front_element` and `back_element` pointers allows future insertion and removal operations to access the corresponding ends of the queue directly.

---

## Memory Layout

Unlike a dynamic array, the queue does not require all elements to occupy a contiguous memory region.

Each element is stored inside an independently allocated node:

```
front_element
      |
      ▼
+------------------+
|   next_element   | ----------------+
+------------------+                 |
|   element data   |                 |
+------------------+                 |
                                     ▼
                              +------------------+
                              |   next_element   | ------+
                              +------------------+       |
                              |   element data   |       |
                              +------------------+       |
                                                         ▼
                                                +------------------+
                                                |   next_element   | —▶ NULL
                                                +------------------+
                                                |   element data   |
                                                +------------------+
                                                                  ▲
                                                                  |
                                                           back_element
```

Each node contains both its linkage information and its element data.

The element storage uses a **flexible array member**:

```c
unsigned char data[];
```

This allows the node allocation to contain both the node metadata and the exact number of bytes required for the stored element.

The required allocation size for a node is conceptually:

```c
sizeof(AtlasQueueElement) + type_size
```

The queue itself stores pointers to both the first and last elements.

> [!NOTE]  
> The queue does not allocate element storage during creation. Memory for individual elements is allocated only when nodes are inserted into the queue.

---

## Current AtlasDS Implementation

The current implementation establishes the queue lifecycle and its internal memory management foundation.

Current capabilities include:

- Generic type-agnostic storage using raw bytes
- Explicit element size tracking (`type_size`)
- Dynamically allocated node-based storage
- Singly linked node structure
- Front and back element tracking
- Empty queue initialization
- Queue size tracking
- Safe queue destruction
- Complete cleanup of all allocated nodes
- Double-pointer destruction to prevent dangling pointers
- Validation of invalid element sizes
- Validation of NULL pointers
- Allocation failure handling

The queue currently provides only its lifecycle operations. Element insertion, removal, and access operations will be added progressively as the implementation evolves.

---

## Currently Implemented API

The current public API consists of the following operations:

```c
AtlasQueue *atlas_queue_create(size_t type_size);

int atlas_queue_destroy(AtlasQueue **ptr_atlas_queue);
```

### `atlas_queue_create()`

Creates and initializes an empty generic queue.

The `type_size` parameter defines the number of bytes required to store each element.

The function:

- Validates that `type_size` is greater than zero
- Allocates the queue structure
- Stores the element size
- Initializes the queue size to zero
- Initializes the front element pointer to `NULL`
- Initializes the back element pointer to `NULL`

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes queue creation to fail and the function returns `NULL`.

The queue initially contains no elements.

After successful creation, the empty-state invariants are:

```c
queue_size == 0
front_element == NULL
back_element == NULL
```

The queue structure itself is allocated, but no element nodes are allocated during creation.

### `atlas_queue_destroy()`

Destroys the queue and releases all memory associated with it.

The function receives a pointer to the user's `AtlasQueue` pointer:

```c
AtlasQueue **ptr_atlas_queue
```

This allows the implementation to invalidate the caller's pointer after releasing the allocated memory.

The destruction process is:

1. Validate the pointer to the queue pointer
2. Validate the referenced queue
3. Traverse the queue from the front element
4. Store the next element before releasing the current node
5. Release every allocated node
6. Release the queue structure itself
7. Set the caller's pointer to `NULL`

The internal traversal follows the `next_element` pointer of each node until `NULL` is reached.

> [!NOTE]  
> The double-pointer interface prevents the caller from retaining a dangling pointer after successful destruction.

If either the provided pointer or the referenced queue is `NULL`, the function returns `ATLAS_ERROR_NULL`.

---

## Safety Guarantees

Because the queue uses generic raw memory and manual dynamic allocation, the implementation performs explicit runtime validation where appropriate.

Current safety mechanisms include:

- NULL pointer validation
- Invalid element size validation
- Allocation failure handling
- Safe destruction of allocated nodes
- Complete cleanup during destruction
- Pointer invalidation after destruction
- Preservation of the queue structure during internal element cleanup

> [!NOTE]  
> These mechanisms provide defensive behavior around the queue lifecycle while preserving the low-level nature of the implementation.

---

## Responsibilities

Using a generic queue requires the caller to provide correct type information and manage the lifetime of the queue correctly.

Core responsibilities currently include:

- Providing a valid element size (`type_size`) during creation
- Ensuring `type_size` represents the actual size of the elements that will be stored
- Checking whether queue creation succeeded
- Destroying queues when they are no longer required
- Passing valid queue pointers to public operations

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
> Queue creation performs a constant amount of work because only the queue metadata is allocated and initialized.

> [!NOTE]  
> Queue destruction traverses every allocated node to release its memory, resulting in O(n) time complexity, where n is the number of stored elements.

Future queue operations will be added to this table as the API expands.

---

## Applications

Queues are commonly used as building blocks for:

- Task scheduling
- Breadth-first search
- Event processing
- Producer-consumer systems
- Message processing
- Request handling
- Buffer management
- Operating system scheduling
- Graph traversal

Queues are particularly useful when elements must be processed in the same order in which they were inserted.

The FIFO model makes queues a fundamental abstraction in computer science and systems programming.

> [!IMPORTANT]  
> The AtlasDS queue implementation is under active development. Additional operations such as insertion, removal, front access, back access, state queries, and clearing will be added progressively.