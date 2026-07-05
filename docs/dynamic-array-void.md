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

> [!IMPORTANT]  
> Since pointer arithmetic is not defined for `void*`, AtlasDS internally converts the base pointer to `char*`, allowing address calculations to be performed one byte at a time.

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
- Automatic capacity growth during insertion
- Manual capacity reservation (`reserve`)
- Logical clearing without deallocation (`clear`)
- Capacity reduction to fit current size (`shrink_to_fit`)
- Size queries (`size`)
- Capacity queries (`capacity`)
- Empty-state queries (`empty`)
- Indexed element access (`get`)
- Indexed element mutation (`set`)
- First element access (`front`)
- Last element access (`back`)
- Stack-like insertion (`push`)
- Stack-like removal (`pop`)
- Indexed insertion (`insert`)
- Indexed removal (`erase`)
- Bounds-checked indexed access
- Automated tests

### Currently Implemented API

```c
AtlasArrayVoid *atlas_array_void_create(size_t type_size, size_t initial_capacity);

int atlas_array_void_destroy(AtlasArrayVoid **ptr_atlas_array_void);

int atlas_array_void_push(AtlasArrayVoid *arr, const void *value);

int atlas_array_void_insert(AtlasArrayVoid *arr, size_t index, const void *value);

int atlas_array_void_erase(AtlasArrayVoid *arr, size_t index);

int atlas_array_void_get(const AtlasArrayVoid *arr, size_t index, void *out_value);

int atlas_array_void_set(AtlasArrayVoid *arr, size_t index, const void *new_value);

int atlas_array_void_pop(AtlasArrayVoid *arr, void *out_value);

int atlas_array_void_size(const AtlasArrayVoid *arr, size_t *out_value);

int atlas_array_void_capacity(const AtlasArrayVoid *arr, size_t *out_value);

int atlas_array_void_empty(const AtlasArrayVoid *arr, bool *out_value);

int atlas_array_void_front(const AtlasArrayVoid *arr, void *out_value);

int atlas_array_void_back(const AtlasArrayVoid *arr, void *out_value);

int atlas_array_void_reserve(AtlasArrayVoid *arr, size_t new_capacity);

int atlas_array_void_clear(AtlasArrayVoid *arr);

int atlas_array_void_shrink_to_fit(AtlasArrayVoid *arr);
```

> [!IMPORTANT]  
> The `type_size` parameter must be greater than zero. Passing `0` causes creation to fail and the function returns `NULL`.

> [!NOTE]  
> If `initial_capacity` is `0`, the array is created with a capacity of `ATLAS_ARRAY_VOID_STANDARD_CAPACITY` elements.

> [!NOTE]  
> The `destroy()` function uses a double pointer to safely invalidate the caller's pointer after releasing the allocated memory.

> [!NOTE]    
> Both `get()` and `set()` validate the requested index against the current logical size (`size`), not the allocated capacity, preventing access to uninitialized memory.

> [!NOTE]  
> The metadata operations (`size`, `capacity`, and `empty`) are constant-time (`O(1)`) queries that never modify the container and simply expose its current logical state.

> [!NOTE]  
> The `front()` and `back()` operations return copies of the first and last stored elements, respectively. Both functions validate that the array is not empty before accessing the internal storage.

> [!NOTE]  
> `reserve()` increases the storage capacity only when the requested capacity is greater than the current one. Requests for smaller or equal capacities perform no operation and still return success.

> [!NOTE]  
> `clear()` removes all stored elements by resetting the logical size to zero while preserving the allocated storage capacity for future insertions.

> [!NOTE]  
> `shrink_to_fit()` reduces the allocated storage capacity to match the current logical size. If the array is empty, the capacity becomes `ATLAS_ARRAY_VOID_STANDARD_CAPACITY`.

> [!NOTE]  
> `insert()` inserts an element at the specified index, shifting all subsequent elements one position to the right while preserving contiguous storage. Insertion at `size` is equivalent to appending a new element.

> [!NOTE]  
> `erase()` removes the element at the specified index and shifts all subsequent elements one position to the left, preserving contiguous storage.

---

## Safety Guarantees

Since `void*` removes compile-time type information, the implementation includes defensive runtime validation to improve robustness.

Implemented safety mechanisms include:

- NULL pointer validation
- Ordered NULL pointer validation during destruction
- Safe destruction using double pointers
- Allocation failure handling
- Memory leak prevention during initialization
- Safe automatic resizing
- Idempotent capacity reservation
- Safe logical clearing without deallocation
- Safe capacity shrinking
- Safe metadata queries (`size`, `capacity`, `empty`)
- Bounds-checked access for indexed operations (`get` / `set`)
- Bounds-checked insertion (`insert`)
- Bounds-checked removal (`erase`)
- Empty-array validation for `pop`, `front`, and `back` operations

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
- Providing valid output buffers for metadata queries (`size`, `capacity`, and `empty`)
- Providing a valid index and output buffer when using `get()`
- Providing a valid index and source object address when using `set()`
- Providing a valid destination buffer when using `front()` or `back()`
- Providing valid object addresses when using `push()`
- Providing a valid destination buffer when using `pop()`
- Providing a valid index and source object address when using `insert()`
- Providing a valid index when using `erase()`
- Providing a valid array when using `reserve()`
- Providing a valid array when using `clear()`
- Providing a valid array when using `shrink_to_fit()`

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
| Size (`size`)            | O(1)            |
| Capacity (`capacity`)    | O(1)            |
| Empty (`empty`)          | O(1)            |
| Access (`get`)           | O(1)            |
| Mutation (`set`)         | O(1)            |
| Front (`front`)          | O(1)            |
| Back (`back`)            | O(1)            |
| Insertion (`push`)       | O(1) amortized  |
| Removal (`pop`)          | O(1)            |
| Insertion (`insert`)     | O(n) worst-case |
| Removal (`erase`)        | O(n) worst-case |
| Reserve (`reserve`)      | O(n)            |
| Clear (`clear`)          | O(1)            |
| Shrink (`shrink_to_fit`) | O(n)            |

> [!NOTE]  
> Creation performs memory allocation proportional to the requested capacity because the storage buffer is allocated during initialization. Likewise, `reserve()` and `shrink_to_fit()` may perform memory reallocation and therefore have linear complexity with respect to the number of stored elements. The `insert()` and `erase()` operations have **O(n)** worst-case complexity because they may need to shift all subsequent elements to preserve contiguous storage.

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