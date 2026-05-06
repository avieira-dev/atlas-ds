# AtlasDS Documentation

This is the official documentation for AtlasDS.

AtlasDS is a low-level data structure library written in pure C, focused on exposing internal behavior such as memory layout, pointer relationships, and structural invariants.

> [!NOTE]  
> For a general overview and installation instructions, see the [README](../README.md).

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

This documentation is organized by modules. Each data structure has its own section containing:

- **Overview**
- **Internal design**
- **Memory behavior**
- **API reference**
- **Usage examples**

---

## Available Modules

- [Dynamic Array](dynamic-array.md)
- _More modules coming soon_

---

## Design Philosophy

AtlasDS follows a few core principles:

- Explicit memory management
- Transparent implementation
- No hidden abstractions (behavior is explicit and predictable)

---

## Project Status

> [!IMPORTANT]  
> The project is **under active development**. APIs may change as the library evolves.

---

## Conventions

AtlasDS uses a consistent `atlas_` prefix for all public APIs to avoid naming collisions and improve clarity.

---

## Where to Start

Start with the **[Dynamic Array](dynamic-array.md)** module