cmake_minimum_required(VERSION 3.10)
project(pseudosquares_prime_sieve CXX)

# Build in release mode by default
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

find_package(OpenMP REQUIRED)

# Find FLINT
find_path(FLINT_INCLUDE_DIR flint/fmpz.h)
find_library(FLINT_LIBRARY NAMES flint)

if(NOT FLINT_INCLUDE_DIR OR NOT FLINT_LIBRARY)
    message(FATAL_ERROR "FLINT C library not found. Please install FLINT first.")
endif()

# Source files
set(SRC_FILES main.cpp pseudosquares_prime_sieve.cpp)

# Main executable
add_executable(pseudosquares_prime_sieve ${SRC_FILES})
target_link_libraries(pseudosquares_prime_sieve ${FLINT_LIBRARY} OpenMP::OpenMP_CXX)

# Test executable
enable_testing()
add_executable(tests tests.cpp pseudosquares_prime_sieve.cpp)
target_link_libraries(tests ${FLINT_LIBRARY} OpenMP::OpenMP_CXX)

# Register test with CTest
add_test(NAME tests COMMAND tests)
