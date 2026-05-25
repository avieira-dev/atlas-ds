# ====================
# Test Executable
# ====================
add_executable(test_array
    test_array.c
)

# ====================
# Link Library
# ====================
target_link_libraries(test_array PRIVATE atlasds)