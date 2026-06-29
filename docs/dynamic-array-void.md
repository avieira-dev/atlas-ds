# Generic Dynamic Array (`Void*`)

A **Generic Dynamic Array** (`AtlasArrayVoid`) is a type-agnostic data structure that stores elements of any type in a contiguous block of memory.

Unlike the specialized integer implementation, this version stores raw bytes and relies on explicit element size tracking (`type_size`) together with manual address calculation to provide generic behavior in pure C.

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

The generic dynamic array stores all metadata required to manage a contiguous memory buffer independently of the element type.

The structure is composed of four main components:

- **data** (`void*`): pointer to the contiguous storage buffer
- **type_size** (`size_t`): size in bytes of a single element
- **size** (`size_t`): number of elements currently stored
- **capacity** (`size_t`): maximum number of elements that can be stored before reallocation becomes necessary

> [!NOTE]  
> The total number of allocated bytes is calculated as: `capacity * type_size`

---

## Memory Layout

Because `data` is stored as a `void*`, the compiler cannot perform pointer arithmetic directly.

To access an element at a given index, the byte offset must be calculated manually:

> address = data + (index * type_size)

In practice, the base pointer is first converted to a byte pointer (such as unsigned char*) before the offset is applied.

Data movement between elements is performed through standard memory-copy operations such as `memcpy`, allowing the container to work with any trivially copyable object.

---

## Current AtlasDS Implementation

The current implementation focuses on building a safe and reliable foundation for future generic operations.

Current capabilities include:

- Manual allocation of the control structure and storage buffer
- Explicit element size tracking (`type_size`)
- Contiguous raw memory storage
- Safe lifecycle management (`create` / `destroy`)
- Automatic minimum-capacity fallback
- Defensive NULL validation
- Memory leak prevention during allocation failures

### Currently Implemented API

```c
AtlasArrayVoid *atlas_array_void_create(size_t type_size, size_t initial_capacity);

int atlas_array_void_destroy(AtlasArrayVoid **ptr_atlas_array_void);
```

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes creation to fail and the function returns `NULL`.

> [!NOTE]  
> If `initial_capacity` is `0`, the array is created with a capacity of `ATLAS_ARRAY_VOID_STANDARD_CAPACITY` elements.

> [!NOTE]  
> The `destroy()` function uses a double pointer to safely invalidate the caller's pointer after releasing the allocated memory.

---

## Safety Guarantees

Since `void*` removes compile-time type information, the implementation includes defensive runtime validation to improve robustness.

Implemented safety mechanisms include:

- NULL pointer validation
- Safe destruction using double pointers
- Allocation failure handling
- Memory leak prevention during initialization
- Ordered NULL pointer validation during destruction

> [!NOTE]  
> These checks are designed to improve stability and predictability while manipulating raw memory.

---

## Responsibilities

Using a generic dynamic array requires explicit memory management and correct type information.

Core responsibilities include:

- Providing the correct element size (`type_size`) during creation
- Allocation (`malloc`)
- Reallocation (`realloc`)
- Deallocation (`free`)

Incorrect usage may lead to:

- Memory corruption
- Invalid memory access
- Undefined behavior
- Dangling pointers

AtlasDS intentionally exposes these responsibilities to help developers understand how generic containers operate internally.

---

## Complexity

| **Operation**            | **Complexity**  |
|:-------------------------|:----------------|
| Creation (`create`)      | O(n)            |
| Destruction (`destroy`)  | O(1)            |

> [!NOTE]  
> Creation performs memory allocation proportional to the requested capacity because the storage buffer is allocated during initialization.

---

## Applications

Generic dynamic arrays are commonly used as the foundation for many low-level systems, including:

- Generic collections
- Serialization buffers
- Binary data processing
- Runtime object storage
- Custom allocators
- Entity-component systems (ECS)
- Systems programming

Generic dynamic arrays are often the underlying storage mechanism for higher-level containers such as vectors, stacks, queues, and hash tables.