# AtlasDS

> A low-level, memory-oriented data structures library in pure C, designed to expose how data structures truly work under the hood.
> Designed for learning, experimentation, and systems-level understanding.

<p align="center">
  <img src="https://img.shields.io/badge/status-in%20development-f39c12"/>
  <img src="https://img.shields.io/badge/language-C-A8B9CC"/>
  <img src="https://img.shields.io/badge/build-CMake-8a63d2"/>
  <img src="https://img.shields.io/badge/license-MIT-6e7781"/>
</p>

> [!NOTE]  
> New data structures will be implemented progressively as the atlas grows.

### Table of Contents

- [Overview](#overview)
- [Quick Example](#quick-example)
- [Philosophy](#philosophy)
- [Naming Convention](#naming-convention)
- [Project Goals](#project-goals)
- [Project Status](#project-status)
- [Planned Structures](#planned-structures)
- [Repository Structure](#repository-structure)
- [Build](#build)
- [Integration](#integration)
- [Developer](#developer)
- [License](#license)

## Overview

The project explores how **fundamental data structures** behave from a low-level systems perspective, exposing their internal mechanics such as memory layout, pointer relationships, and structural invariants.

Rather than providing high-level abstractions, AtlasDS focuses on transparent implementations that reveal how data structures operate internally.

## Quick Example

> The following example illustrates the intended usage of AtlasDS.  
> The API may evolve as the project is under active development.

```c
#include <atlas/array.h> 

int main() { 

    atlas_array* arr = atlas_array_create(sizeof(int)); 

    int value = 10; 
    atlas_array_push(arr, &value); 
    
    int* data = (int*)atlas_array_get(arr, 0); 
    
    atlas_array_destroy(arr); 
    return 0; 
}
```

## Philosophy

Each structure in AtlasDS is implemented following a few core principles: 

- Implemented entirely from scratch
- Explicit control over memory allocation
- Typical usage patterns found in systems programming

The goal is to treat each structure not merely as a container, but as a mechanical system of memory.

## Naming Convention

AtlasDS uses a consistent `atlas_` prefix for all public APIs to ensure clarity and avoid symbol collisions in C programs.

## Project Goals

AtlasDS is designed as a technical exploration of:

- Low-level implementation techniques in C
- Memory organization inside data structures
- Pointer-based relationships between elements

> [!NOTE]  
> The repository also serves as a reference implementation library for students and developers studying computer science fundamentals.

## Project Status

- Core structures: in progress
- API stability: unstable
- Production readiness: no

## Planned Structures

The atlas will progressively include implementations of structures such as:

- Dynamic Arrays
- Linked Lists
- Stacks
- Queues
- Hash Tables
- Binary Search Trees
- Heaps / Priority Queues
- Graph Representations

Each module will contain:

- The implementation
- Usage examples
- Documentation
- Automated tests

## Repository Structure

```text
atlas-ds
├── build/
├── docs/
├── examples/
├── include/
│   └── atlas/
├── src/
├── tests/
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## Build

AtlasDS uses CMake as its build system.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Integration

AtlasDS can be integrated into your project by cloning the repository into your project directory.

### Clone into your project

```bash
git clone https://github.com/avieira-dev/atlas-ds.git
```

Example project structure:

```text
your-project/
├── atlas/
├── src/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

### Using with CMake

Add AtlasDS to your project:

```cmake
add_subdirectory(atlas-ds)
target_link_libraries(your_project atlas)
```

> [!NOTE]  
> This is currently the recommended integration method.  
> Additional methods may be supported in the future.

## Developer
**Alexandre Vieira**  
GitHub: [@avieira-dev](https://github.com/avieira-dev)

## License
Distributed under the license [MIT License](LICENSE). See the **LICENSE** file for more details.