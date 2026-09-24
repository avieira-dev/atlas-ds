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
  - [atlas_stack_create()](#atlas_stack_create)
  - [atlas_stack_destroy()](#atlas_stack_destroy)
  - [atlas_stack_push()](#atlas_stack_push)
  - [atlas_stack_pop()](#atlas_stack_pop)
  - [atlas_stack_top()](#atlas_stack_top)
  - [atlas_stack_size()](#atlas_stack_size)
  - [atlas_stack_empty()](#atlas_stack_empty)
  - [atlas_stack_clear()](#atlas_stack_clear)
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

The current implementation provides the lifecycle and core LIFO operations required to manage a generic stack.

Current capabilities include:

- Generic type-agnostic storage using raw bytes
- Explicit element size tracking (`type_size`)
- Dynamically allocated node-based storage
- Singly linked node structure
- Top element tracking
- Empty stack initialization
- Stack size tracking
- Stack insertion (`push`)
- Stack removal (`pop`)
- Stack top element access (`top`)
- Stack size queries (`size`)
- Empty-state queries (`empty`)
- Stack clearing while preserving the stack structure (`clear`)
- LIFO element ordering
- Safe stack destruction
- Complete cleanup of all allocated nodes
- Double-pointer destruction to prevent dangling pointers
- Validation of invalid element sizes
- Validation of NULL pointers
- Empty-stack validation during removal and top access
- Allocation failure handling
- Automated lifecycle, insertion, removal, access, state, and maintenance tests

---

## Currently Implemented API

The current public API consists of the following operations:

```c
AtlasStack *atlas_stack_create(size_t type_size);

int atlas_stack_destroy(AtlasStack **ptr_atlas_stack);

int atlas_stack_push(AtlasStack *stack, const void *value);

int atlas_stack_pop(AtlasStack *stack, void *out_value);

int atlas_stack_top(const AtlasStack *stack, void *out_value);

int atlas_stack_size(const AtlasStack *stack, size_t *out_value);

int atlas_stack_empty(const AtlasStack *stack, bool *out_value);

int atlas_stack_clear(AtlasStack *stack);
```

### `atlas_stack_create()`

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

### `atlas_stack_destroy()`

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

### `atlas_stack_push()`

Inserts a new element at the top of the stack.

The function receives the stack and a pointer to the value that will be copied into the newly allocated node:

```c
int atlas_stack_push(AtlasStack *stack, const void *value);
```

The insertion process is:

- Validate the stack pointer.
- Validate the value pointer.
- Allocate a new node using the stack's `type_size`.
- Copy the element bytes into the node's `data` storage.
- Store the current top element in a temporary pointer.
- Make the new node the stack's top element.
- Set the new node's `previous_element` pointer to the previous top element.
- Increment `stack_size`.

The existing nodes are not moved or reallocated. The new node is simply linked in front of the previous top element.

> [!NOTE]  
> `push()` always inserts at the top of the stack, which is what allows the structure to maintain LIFO behavior.

> [!NOTE]  
> If node allocation fails, the stack remains unchanged and the function returns `ATLAS_ERROR_MEMORY`.

### `atlas_stack_pop()`

Removes the top element from the stack and copies its value into the provided output buffer:

```c
int atlas_stack_pop(AtlasStack *stack, void *out_value);
```

The `out_value` parameter receives a copy of the removed element.

The removal process is:

- Validate the stack pointer
- Validate the output value pointer
- Validate that the stack is not empty
- Store the current top element in a temporary pointer
- Move the stack's top element to the current node's `previous_element`
- Copy the element data into `out_value`
- Release the removed node
- Decrement `stack_size`

The existing nodes are not moved or reallocated. Removing the top element only changes the `top_element` pointer.

> [!NOTE]  
> The `out_value` buffer must provide enough storage for `type_size` bytes. AtlasDS stores raw bytes and cannot verify the size of the caller-provided output buffer.

> [!NOTE]  
> If the stack is empty, `pop()` does not modify the stack and returns `ATLAS_ERROR_EMPTY`.

### `atlas_stack_top()`

Returns the value of the top element without removing it:

```c
int atlas_stack_top(const AtlasStack *stack, void *out_value);
```

The `out_value` parameter receives a copy of the current top element.

The access process is:

- Validate the stack pointer
- Validate the output value pointer
- Validate that the stack is not empty
- Copy the top element data into `out_value`

Unlike `pop()`, the top element is not removed and the stack remains unchanged.

> [!NOTE]  
> The `out_value` buffer must provide enough storage for `type_size` bytes. AtlasDS stores raw bytes and cannot verify the size of the caller-provided output buffer.

>[!NOTE]  
> If the stack is empty, `top()` does not modify the stack and returns `ATLAS_ERROR_EMPTY`.

### `atlas_stack_size()`

Returns the current number of elements stored in the stack without modifying the structure:

```c
int atlas_stack_size(const AtlasStack *stack, size_t *out_value);
```

The `out_value` parameter receives the current number of elements stored in the stack.

The operation:

- Validate the stack pointer
- Validate the output value pointer
- Copy `stack_size` into `out_value`

The stack maintains its element count internally, so the function does not need to traverse the linked structure.

> [!NOTE]  
> `size()` executes in O(1) time because the current number of elements is maintained directly by the stack.

> [!NOTE]  
> An empty stack is valid and returns `ATLAS_SUCCESS` with `out_value` set to `0`.

### `atlas_stack_empty()`

Checks whether the stack currently contains any elements:

```c
int atlas_stack_empty(const AtlasStack *stack, bool *out_value);
```

The `out_value` parameter receives `true` when the stack contains no elements and `false` otherwise.

The operation:

- Validate the stack pointer
- Validate the output value pointer
- Compare the current `stack_size` against zero
- Store the result in `out_value`

The stack itself is not modified.

> [!NOTE]  
> `empty()` executes in O(1) time because the stack size is maintained internally.

> [!NOTE]  
> An empty stack is a valid state. The function returns `ATLAS_SUCCESS` and stores `true` in `out_value`.

### `atlas_stack_clear()`

Removes all elements from the stack while preserving the stack structure itself:

```c
int atlas_stack_clear(AtlasStack *stack);
```

Unlike `destroy()`, this operation does not release the `AtlasStack` structure. After `clear()` completes successfully, the same stack can be reused for new insertions.

The clearing process is:

- Validate the stack pointer
- Traverse the stack from the top element
- Store the previous element before releasing the current node
- Release every allocated node
- Set `top_element` to `NULL`
- Reset `stack_size` to zero

After the operation, the stack satisfies the empty-state invariants:

```text
stack_size == 0
top_element == NULL
```

> [!NOTE]  
> Calling `clear()` on an already empty stack is considered successful and returns `ATLAS_SUCCESS`.

> [!NOTE]  
> `clear()` releases all element nodes but keeps the `AtlasStack` structure allocated and ready for reuse.

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
- Safe clearing of allocated nodes
- Preservation of the stack structure after `clear()`
- Consistent empty-state restoration after `clear()`

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
- Providing valid input values to `push()`
- Providing an output buffer large enough to receive `type_size` bytes when using `pop()`
- Providing an output buffer large enough to receive `type_size` bytes when using `top()`
- Providing valid output buffers for `size()` and `empty()`
- Understanding that `clear()` removes all elements but preserves the stack object

Calling `clear()` does not invalidate the `AtlasStack` pointer. The stack remains valid and can be reused after all elements have been removed.

Incorrect usage of generic raw-memory structures may result in:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Incorrect interpretation of stored bytes
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to demonstrate how manually managed generic data structures operate internally.

---

## Complexity

| Operation                | Complexity |
|:-------------------------|:-----------|
| Creation (`create`)      | O(1)       |
| Destruction (`destroy`)  | O(n)       |
| Push (`push`)            | O(1)       |
| Pop (`pop`)              | O(1)       |
| Top (`top`)              | O(1)       |
| Size (`size`)            | O(1)       |
| Empty (`empty`)          | O(1)       |
| Clear (`clear`)          | O(n)       |

> [!NOTE]  
> Stack creation performs a constant amount of work because only the stack metadata is allocated and initialized.

> [!NOTE]  
> Stack destruction traverses every allocated node to release its memory, resulting in O(n) time complexity, where `n` is the number of stored elements.

> [!NOTE]  
> `push()` executes in O(1) time because the new element is inserted directly at the top of the stack. No traversal of existing elements is required.

> [!NOTE]  
> `pop()` executes in O(1) time because the top element is removed directly through the `top_element` pointer. No traversal of the remaining elements is required.

> [!NOTE]  
> `top()` executes in O(1) time because the current top element is accessed directly through the `top_element` pointer without modifying or traversing the stack.

> [!NOTE]  
> `size()` executes in O(1) time because the stack maintains its current element count internally.

> [!NOTE]  
> `empty()` executes in O(1) time because it only checks whether the internally maintained stack size is zero.

> [!NOTE]  
> `clear()` executes in O(n) time because every allocated element node must be traversed and released.

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

The following example demonstrates how to create a generic stack, insert elements using `push()`, inspect its state using `size()` and `empty()`, access the top element using `top()`, remove it using `pop()`, and clear the remaining elements using `clear()`.

```c
#include <stdbool.h>

#include "atlas/stack.h" 
#include "atlas/status.h"

int main(void) {
    AtlasStack *stack = atlas_stack_create(sizeof(int));
    if (!stack) {
        return 1;
    }

    int first = 5;
    int second = 10;
    int third = 15;

    atlas_stack_push(stack, &first); 
    atlas_stack_push(stack, &second); 
    atlas_stack_push(stack, &third);

    int out_value;

    if (atlas_stack_top(stack, &out_value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (out_value != third) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_pop(stack, &out_value) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    size_t size = 0;

    if (atlas_stack_size(stack, &size) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (size != 2) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    bool empty = false;

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_clear(stack) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_empty(stack, &empty) != ATLAS_SUCCESS) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (!empty) {
        atlas_stack_destroy(&stack);
        return 1;
    }

    if (atlas_stack_destroy(&stack) != ATLAS_SUCCESS) {
        return 1;
    }

    return 0;
}
```