# AtlasDS Documentation

This is the official documentation for AtlasDS.

AtlasDS is a low-level data structures library written in pure C, focused on exposing the internal mechanics of fundamental data structures, including memory layout, pointer relationships, allocation strategies, and structural invariants.

> [!NOTE]  
> For a general overview, build instructions, and project integration, see the [README](../README.md).

---

## Table of Contents

- [Documentation Structure](#documentation-structure)
- [Available Modules](#available-modules)
- [Design Philosophy](#design-philosophy)
- [Project Status](#project-status)
- [Conventions](#conventions)
- [Where to Start](#where-to-start)

---

## Documentation Structure

The documentation is organized by data structure modules. As AtlasDS evolves, each module is expanded with implementation details and practical usage information.

Each module typically includes:

- **Overview**
- **Conceptual design**
- **Memory behavior**
- **API reference**
- **Safety guarantees**
- **Responsibilities**
- **Complexity analysis**
- **Usage examples**

---

## Available Modules

- [Dynamic Array](dynamic-array.md)
- [Generic Dynamic Array](dynamic-array-void.md)
- [Generic Linked List](docs/list.md)

_More data structure modules will be added as AtlasDS continues to evolve._

---

## Design Philosophy

AtlasDS follows a few core principles:

- Explicit memory management
- Transparent implementations
- Predictable runtime behavior
- No hidden abstractions

---

## Project Status

> [!IMPORTANT]  
> AtlasDS is **under active development**. New data structures and APIs are added incrementally, and existing interfaces may evolve over time.

---

## Conventions

AtlasDS uses a consistent `atlas_` prefix for all public symbols to avoid naming collisions and improve API clarity.

Documentation follows the same philosophy as the library itself: every behavior should be explicit, predictable, and easy to understand.

---

## Where to Start

If you are new to AtlasDS, the following modules provide a solid introduction to the library's design principles:

- **[Dynamic Array](dynamic-array.md)** — fixed-type implementation focused on integer storage.
- **[Generic Dynamic Array](dynamic-array-void.md)** — type-agnostic implementation based on raw memory manipulation.
- **[Generic Linked List](list.md)** — generic node-based container illustrating dynamic memory allocation and pointer-based data structures.

Together, these modules introduce the core design patterns used throughout AtlasDS.