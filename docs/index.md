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
- **Complexity analysis**
- **Usage examples**

---

## Available Modules

- [Dynamic Array (`int`)](dynamic-array.md)
- [Generic Dynamic Array (`void*`)](dynamic-array-void.md)

_More modules will be added as AtlasDS grows._

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

If you are new to AtlasDS, begin with one of the Dynamic Array modules:

- **[Dynamic Array (`int`)](dynamic-array.md)** — fixed-type implementation focused on integer storage.
- **[Generic Dynamic Array (`void*`)](dynamic-array-void.md)** — type-agnostic implementation based on raw memory manipulation.

These modules introduce the core design patterns used throughout the library.