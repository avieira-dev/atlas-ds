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

---

## Conceptual Structure

The generic linked list stores the metadata required to manage a collection of dynamically allocated nodes independently of the stored element type.

The structure is composed of four main components:

- **type_size** (`size_t`): size in bytes of each stored element
- **list_size** (`size_t`): number of nodes currently stored in the list
- **first_node** (`AtlasListNode*`): pointer to the first node of the list
- **last_node** (`AtlasListNode*`): pointer to the last node of the list

Each node contains:

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
first_node
    |
    v
+------+     +------+     +------+
| Node | --> | Node | --> | Node |
+------+     +------+     +------+
                            |
                            v
                       last_node
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
- Safe list destruction
- Complete node cleanup during destruction
- Double-pointer destruction to prevent dangling pointers
- Defensive NULL validation

### Currently Implemented API

```c
AtlasList *atlas_list_create(size_t type_size);

int atlas_list_destroy(AtlasList **ptr_atlas_list);
```

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes creation to fail and the function returns `NULL`.

> [!NOTE]  
> The list initially contains no nodes. Nodes are created only when insertion operations are implemented.

> [!NOTE]  
> The `destroy()` function traverses the entire linked structure, releases each allocated node, then releases the list structure itself.

> [!NOTE]  
> The `destroy()` function uses a double pointer to safely invalidate the caller's pointer after releasing memory.

---

## Safety Guarantees

Since `void*` removes compile-time type information, the implementation relies on explicit runtime validation.

Implemented safety mechanisms include:

- NULL pointer validation
- Invalid element size validation
- Allocation failure handling
- Memory leak prevention during initialization
- Safe destruction of all allocated nodes
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
- Providing valid list pointers when calling operations
- Ensuring future comparison callbacks correctly interpret stored element types

Incorrect usage may lead to:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to demonstrate how linked structures operate internally.

---

## Complexity

| Operation | Complexity |
|:----------|:-----------|
| Creation (`create`) | O(1) |
| Destruction (`destroy`) | O(n) |

> [!NOTE]  
> The destruction operation is linear because every allocated node must be individually traversed and released.

Future operations will introduce additional complexity analysis as the API expands.

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
> The linked list implementation is under active development. Additional operations such as insertion, removal, traversal, searching, copying, and cloning will be added progressively.