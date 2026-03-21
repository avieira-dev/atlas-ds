# AtlasDS — Data Structures in C

> AtlasDS is a structured collection of fundamental data structures implemented in pure C.


![status](https://img.shields.io/badge/status-in%20development-orange)
![language](https://img.shields.io/badge/language-C-blue)
![build](https://img.shields.io/badge/build-CMake-red)
![license](https://img.shields.io/badge/license-MIT-blue)

> [!NOTE]  
> New data structures will be implemented progressively as the atlas grows.

### Table of Contents
- [Overview](#overview)
- [Philosophy](#philosophy)
- [Project Goals](#project-goals)
- [Planned Structures](#planned-structures)
- [Repository Structure](#repository-structure)
- [Build](#build)
- [Developer](#developer)
- [License](#license)

## Overview
The project explores how **classic data structures** behave from a low-level systems perspective, **exposing their internal mechanics** such as memory layout, pointer relationships, and structural invariants.

Rather than providing high-level abstractions, **AtlasDS focuses on transparent implementations** that reveal how data structures operate internally.

## Philosophy
Each structure in AtlasDS is implemented following a few core principles: 

- Implemented entirely from scratch
- Explicit control over memory allocation
- Typical usage patterns found in systems programming

The goal is to treat each structure not merely as a container, but as a mechanical system of memory.

## Project Goals
AtlasDS is designed as a technical exploration of:

- Low-level implementation techniques in C
- Memory organization inside data structures
- Pointer-based relationships between elements

> [!NOTE]  
> The repository also serves as a reference implementation library
> for students and developers studying computer science fundamentals.

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

## Developer
**Alexandre Vieira**  
GitHub: [@avieira-dev](https://github.com/avieira-dev)

## License
Distributed under the license [MIT License](LICENSE). See the **LICENSE** file for more details.