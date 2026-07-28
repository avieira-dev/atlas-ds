# Generic Linked List (`Void*`)

A **Generic Linked List** (`AtlasList`) is a type-agnostic linked data structure that stores elements of any type through dynamically allocated nodes.

Unlike dynamic arrays, linked lists do not store elements in contiguous memory. Instead, each element is placed inside an independent node connected through pointers, allowing dynamic growth without requiring memory reallocation of the entire structure.

The current AtlasDS implementation uses a **singly linked list**, where each node stores a pointer to the next node and a flexible memory region containing the element data.

---

## Table of Contents

- [Conceptual Structure](#conceptual-structure)
- [Memory Layout](#memory-layout)
- [Current AtlasDS Implementation](#current-atlasds-implementation)
- [Safety Guarantees](#safety-guarantees)
- [Responsibilities](#responsibilities)
- [Complexity](#complexity)
- [Applications](#applications)
- [Usage Example](#usage-example)

---

## Conceptual Structure

The generic linked list stores the metadata required to manage a collection of dynamically allocated nodes independently of the stored element type.

The `AtlasList` structure maintains four pieces of metadata required to manage the linked list:

- **type_size** (`size_t`): size in bytes of each stored element
- **list_size** (`size_t`): number of nodes currently stored in the list
- **first_node** (`AtlasListNode*`): pointer to the first node of the list
- **last_node** (`AtlasListNode*`): pointer to the last node of the list

Each `AtlasListNode` contains:

- **next_node** (`AtlasListNode*`): pointer to the next node in the chain
- **data** (`unsigned char[]`): flexible storage area containing the element bytes

> [!NOTE]  
> The list does not allocate memory for elements in advance. Memory is allocated only when new nodes are created.

---

## Memory Layout

Unlike dynamic arrays, linked lists do not use a contiguous memory buffer.

Each node is allocated independently and contains:

```text
+----------------+
| next_node      | ---> next node
+----------------+
| element data   |
+----------------+
```

The list maintains references to the beginning and end of the chain:

```text
first_node                last_node
    |                        |
    v                        v
+------+     +------+     +------+
| Node | --> | Node | --> | Node | ---+
+------+     +------+     +------+    |
                                      |
                                      v
                                     NULL
```

The element storage uses a flexible array member:

```c
unsigned char data[];
```

This allows each node allocation to contain both the node metadata and the stored element bytes in a single memory block.

The required allocation size for a node is:

```text
sizeof(AtlasListNode) + type_size
```

---

## Current AtlasDS Implementation

The current implementation focuses on establishing a safe foundation for future linked list operations.

Current capabilities include:

- Generic type-agnostic storage using raw bytes
- Explicit element size tracking (`type_size`)
- Dynamic node allocation model
- Singly linked node structure
- First and last node tracking
- Empty list initialization
- Size and empty-state queries (`size`, `empty`)
- First (`front`) and last (`back`) element access
- Indexed element access (`get`)
- Indexed element mutation (`set`)
- Indexed element swapping (`swap`)
- Front insertion (`push_front`)
- Back insertion (`push_back`)
- Indexed insertion (`insert`)
- Front removal (`pop_front`)
- Back removal (`pop_back`)
- Indexed removal (`erase`)
- Removal of all elements while preserving the list structure (`clear`)
- Safe list destruction
- Complete cleanup of all allocated nodes
- Double-pointer destruction to prevent dangling pointers
- Defensive NULL validation

### Currently Implemented API

```c
AtlasList *atlas_list_create(size_t type_size);

int atlas_list_destroy(AtlasList **ptr_atlas_list);

int atlas_list_push_front(AtlasList *list, const void *value);

int atlas_list_push_back(AtlasList *list, const void *value);

int atlas_list_pop_front(AtlasList *list, void *out_value);

int atlas_list_pop_back(AtlasList *list, void *out_value);

int atlas_list_get(const AtlasList *list, void *out_value, size_t index);

int atlas_list_set(AtlasList *list, const void *new_value, size_t index);

int atlas_list_size(const AtlasList *list, size_t *out_value);

int atlas_list_empty(const AtlasList *list, bool *out_value);

int atlas_list_front(const AtlasList *list, void *out_value);

int atlas_list_back(const AtlasList *list, void *out_value);

int atlas_list_insert(AtlasList *list, size_t index, const void *value);

int atlas_list_erase(AtlasList *list, size_t index, void *out_value);

int atlas_list_clear(AtlasList *list);

int atlas_list_swap(const AtlasList *list, size_t index_a, size_t index_b);
```

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes creation to fail and the function returns `NULL`.

> [!NOTE]  
> The list initially contains no nodes. Nodes are created dynamically when insertion operations are performed.

> [!NOTE]  
> The `destroy()` function traverses the entire linked structure, releases each allocated node, then releases the list structure itself.

> [!NOTE]  
> The `destroy()` function uses a double pointer to safely invalidate the caller's pointer after releasing memory.

> [!NOTE]  
> The `push_front()` operation allocates a new node and inserts it at the beginning of the list. The previous first node becomes the next node of the inserted element.

> [!NOTE]  
> The `push_back()` operation allocates a new node and inserts it at the end of the list. The new node becomes the last element and the previous last node points to it.

> [!NOTE]  
> The `pop_front()` operation copies the value stored in the first node into the caller-provided output buffer, removes the node from the list, and releases its allocated memory.

> [!NOTE]  
> The `pop_back()` operation copies the value stored in the last node into the caller-provided output buffer, removes the node from the list, and releases its allocated memory. Because the current implementation uses a singly linked list, the operation traverses the list to locate the node preceding the last one.

> [!NOTE]  
> The `get()` operation traverses the list until reaching the specified zero-based index, then copies the stored element into the caller-provided output buffer.

> [!NOTE]  
> The `set()` operation traverses the list until reaching the specified zero-based index, then replaces the stored element with the provided value by copying it into the node's internal storage.

> [!NOTE]  
> The `size()` operation returns the current number of elements stored in the list through a user-provided output pointer. Since the list maintains its size internally, the operation executes in constant time.

> [!NOTE]  
> The `empty()` operation reports whether the list contains any elements by comparing the internally maintained size against zero. The result is written to a user-provided output pointer and executes in constant time.

> [!NOTE]  
> The `front()` operation copies the element stored in the first node into the caller-provided output buffer without removing it from the list. The operation executes in constant time.

> [!NOTE]  
> The `back()` operation copies the element stored in the last node into the caller-provided output buffer without removing it from the list. The operation executes in constant time.

> [!NOTE]  
> The `insert()` operation inserts a new node before the specified zero-based index. Inserting at index `0` is equivalent to `push_front()`, while inserting at the current list size appends the element and is equivalent to `push_back()`.

> [!NOTE]  
> The `erase()` operation removes the node stored at the specified zero-based index, copies its value into the caller-provided output buffer, and releases the node's allocated memory.

> [!NOTE]  
> The `clear()` operation releases every node currently stored in the list while preserving the list structure itself. After completion, the list becomes empty and is immediately ready to accept new insertions without requiring re-creation.

> [!NOTE]  
> The `swap()` operation traverses the list until reaching the two specified zero-based indices, then exchanges the stored element data between the corresponding nodes. The linked structure itself remains unchanged, since only the contents of the nodes are swapped.

---

## Safety Guarantees

Since generic storage removes compile-time type information, the implementation relies on explicit runtime validation.

Implemented safety mechanisms include:

- NULL pointer validation
- Invalid element size validation
- Bounds checking for indexed operations
- Allocation failure handling
- Memory leak prevention during initialization
- Safe release of all allocated nodes
- Safe pointer invalidation after destruction

> [!NOTE]  
> These checks improve reliability while manipulating manually managed dynamic memory.

---

## Responsibilities

Using a generic linked list requires correct type information and explicit memory management.

Core responsibilities include:

- Providing the correct element size (`type_size`) during creation
- Ensuring inserted elements match the specified element size
- Managing the lifetime of lists created by the API
- Destroying lists when they are no longer needed
- Using `clear()` when removing all elements while preserving the list object
- Providing valid list pointers when calling operations
- Providing valid indices when using indexed operations such as `get()`, `set()`, `swap()`, `insert()`, and `erase()`
- Ensuring future comparison callbacks correctly interpret stored element types

Incorrect usage may lead to:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to demonstrate how linked structures operate internally.

---

## Complexity

| **Operation**                  | **Complexity**  |
|:-------------------------------|:----------------|
| Creation (`create`)            | O(1)            |
| Destruction (`destroy`)        | O(n)            |
| Front insertion (`push_front`) | O(1)            |
| Back insertion (`push_back`)   | O(1)            |
| Front removal (`pop_front`)    | O(1)            |
| Back removal (`pop_back`)      | O(n)            |
| Indexed access (`get`)         | O(n)            |
| Indexed mutation (`set`)       | O(n)            |
| Size (`size`)                  | O(1)            |
| Empty query (`empty`)          | O(1)            |
| First element access (`front`) | O(1)            |
| Last element access (`back`)   | O(1)            |
| Indexed insertion (`insert`)   | O(n)            |
| Indexed removal (`erase`)      | O(n)            |
| Clear (`clear`)                | O(n)            |
| Swap (`swap`)                  | O(n)            |

> [!NOTE]  
> The `destroy()`, `clear()`, `pop_back()`, `get()`, `set()`, `swap()`, `insert()`, and `erase()` operations may require traversing the linked structure and therefore have linear time complexity.

> [!NOTE]  
> The `insert()` operation executes in **O(1)** when inserting at the beginning (`index == 0`) or at the end (`index == list_size`), since these cases delegate to `push_front()` and `push_back()`. Inserting at any other position requires traversing the list to locate the insertion point, resulting in **O(n)** time complexity.

> [!NOTE] 
> The `erase()` operation executes in **O(1)** only when removing the first element (`index == 0`), since it delegates to `pop_front()`. Removing the last element or an intermediate element requires traversing the list to locate the preceding node, resulting in **O(n)** time complexity.


Future operations will extend this table with additional complexity analysis as the API expands.

---

## Applications

Generic linked lists are commonly used as building blocks for:

- Stacks
- Queues
- Graph adjacency lists
- Memory management structures
- Dynamic object collections
- Systems programming

Linked lists are especially useful when frequent insertion and removal operations are required and contiguous memory storage is not the primary requirement.

---

> [!NOTE]  
> The linked list implementation is under active development. Additional operations such as searching, copying, cloning, reversing, and iterator-style utilities will be added progressively.
---

## Usage Example

> The API may evolve as the project is under active development.

```c
#include <atlas/list.h>

#include <stdio.h>
#include <stdbool.h>

int main(void) {
    AtlasList *list = atlas_list_create(sizeof(int));

    if (!list) {
        return 1;
    }

    int a = 10;
    int b = 20;
    int c = 30;

    atlas_list_push_back(list, &a);
    atlas_list_push_back(list, &b);
    atlas_list_push_front(list, &c);

    int inserted = 15;
    atlas_list_insert(list, 1, &inserted);

    int erased_value = 0;
    atlas_list_erase(list, 2, &erased_value);

    int new_value = 99;
    atlas_list_set(list, &new_value, 0);

    int another_value = 42;
    atlas_list_push_back(list, &another_value);

    atlas_list_swap(list, 0, 2);

    int retrieved_value = 0;

    if (atlas_list_get(list, &retrieved_value, 0) != 0) {
        atlas_list_destroy(&list);
        return 1;
    }

    int first = 0;
    int last = 0;

    atlas_list_front(list, &first);
    atlas_list_back(list, &last);

    printf("Front: %d | Back: %d\n", first, last);

    int popped_front = 0;
    int popped_back = 0;

    if (atlas_list_pop_front(list, &popped_front) != 0) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (atlas_list_pop_back(list, &popped_back) != 0) {
        atlas_list_destroy(&list);
        return 1;
    }

    size_t size = 0;
    atlas_list_size(list, &size);

    bool empty_list = false;

    if (atlas_list_empty(list, &empty_list) != 0) {
        atlas_list_destroy(&list);
        return 1;
    }

    if (empty_list) {
        int fallback = 1;
        atlas_list_push_back(list, &fallback);
    }

    atlas_list_clear(list);

    bool empty_after_clear = false;
    atlas_list_empty(list, &empty_after_clear);

    printf("Empty after clear: %s\n", empty_after_clear ? "true" : "false");

    atlas_list_destroy(&list);

    return 0;
}
```