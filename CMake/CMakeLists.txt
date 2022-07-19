cmake_minimum_required(VERSION 3.10)

# set the project name
project(Scratch VERSION 0.1)

target_include_directories(
    Scratch PUBLIC
    "${SCRATCH_BINARY_DIR}/src"
    "${SCRATCH_SOURCE_DIR}/lib"
)

# Add the ext library
add_subdirectory(lib)

# add the executable
add_executable(Scratch main.c)
