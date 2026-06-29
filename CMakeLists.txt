cmake_minimum_required(VERSION 3.28)

project(AtlasDS
    VERSION 0.1
    DESCRIPTION "Atlas of fundamental data structures implemented in C"
    LANGUAGES C
)

# ====================
# C Standard
# ====================
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ====================
# Library
# ====================
add_library(atlasds STATIC
    src/array.c
    src/array_void.c
)

# ====================
# Include Directories
# ====================
target_include_directories(atlasds
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ====================
# Build Options
# ====================
option(ATLAS_BUILD_SCRATCH "Build personal scratch programs" OFF)

# ====================
# Subdirectories
# ====================
add_subdirectory(tests)

if(ATLAS_BUILD_SCRATCH)
    add_subdirectory(scratch)
endif()