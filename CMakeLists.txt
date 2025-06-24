cmake_minimum_required(VERSION 3.10)
project(pseudosquares_prime_sieve CXX)

# Build in release mode by default
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

find_package(Threads REQUIRED QUIET)

# Find GMP C library
find_path(GMP_HEADER gmp.h)
find_library(GMP_LIBRARY NAMES gmp)

if(NOT GMP_HEADER OR NOT GMP_LIBRARY)
    message(FATAL_ERROR "GMP C library not found. Please install the GMP C library.")
endif()

# Source files
set(SRC_FILES main.cpp pseudosquares_prime_sieve.cpp)

# Main executable
add_executable(pseudosquares_prime_sieve ${SRC_FILES})
target_link_libraries(pseudosquares_prime_sieve ${GMP_LIBRARY} Threads::Threads)

# Test executable
enable_testing()
add_executable(tests tests.cpp pseudosquares_prime_sieve.cpp)
target_link_libraries(tests ${GMP_LIBRARY})

# Register test with CTest
add_test(NAME tests COMMAND tests)
