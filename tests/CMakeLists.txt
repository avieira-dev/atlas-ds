# ====================
# Test Executable
# ====================
add_executable(test_array
    test_array.c
)

add_executable(test_array_void
    test_array_void.c
)

# ====================
# Link Library
# ====================
target_link_libraries(test_array PRIVATE atlasds)
target_link_libraries(test_array_void PRIVATE atlasds)