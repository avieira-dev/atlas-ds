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
# Source Files
# ====================
file(GLOB_RECURSE ATLAS_SOURCES
    src/*.c
)

# ====================
# Library
# ====================
add_library(atlasds STATIC ${ATLAS_SOURCES})

# ====================
# Include Directories
# ====================
target_include_directories(atlasds PUBLIC include)

# ====================
# Subdirectories
# ====================
add_subdirectory(examples)
add_subdirectory(tests)