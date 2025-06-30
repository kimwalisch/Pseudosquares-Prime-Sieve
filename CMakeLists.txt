cmake_minimum_required(VERSION 3.10)
project(pseudosquares_prime_sieve CXX)

# Build options ######################################################

# The libprimesieve dependency is present in our project’s source
# tree and it will be built by default. You can also disable
# building libprimesieve and instead install it using your package
# manager.
option(BUILD_LIBPRIMESIEVE "Build libprimesieve" ON)

# Build in release mode by default
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Enable assertions in Debug mode only
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND PRIMECOUNT_COMPILE_DEFINITIONS "ENABLE_ASSERT")
endif()

# libprimesieve ######################################################

# By default the libprimesieve dependency is built from source
# (BUILD_LIBPRIMESIEVE=ON). However when packaging primecount
# for e.g. a Linux distro this is not a good idea. For this use
# case it is better to install the libprimesieve package
# (e.g. libprimesieve-dev) and link against that version.

if(BUILD_LIBPRIMESIEVE)
    set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}")
    set(COPY_BUILD_EXAMPLES "${BUILD_EXAMPLES}")
    set(COPY_BUILD_MANPAGE "${BUILD_MANPAGE}")
    set(COPY_BUILD_TESTS "${BUILD_TESTS}")

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libprimesieve" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "Build example programs" FORCE)
    set(BUILD_MANPAGE OFF CACHE BOOL "Build primesieve manpage" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "Build primesieve tests" FORCE)
    option(BUILD_PRIMESIEVE "Build primesieve binary" OFF)

    add_subdirectory(lib/primesieve)

    set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL "Build shared libprimesieve" FORCE)
    set(BUILD_EXAMPLES "${COPY_BUILD_EXAMPLES}" CACHE BOOL "Build example programs" FORCE)
    set(BUILD_MANPAGE "${COPY_BUILD_MANPAGE}" CACHE BOOL "Regenerate man page using a2x" FORCE)
    set(BUILD_TESTS "${COPY_BUILD_TESTS}" CACHE BOOL "Build test programs" FORCE)
else()
    find_package(primesieve REQUIRED)

    if(primesieve_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "Found libprimesive-${primesieve_VERSION}, but primecount requires libprimesive >= 11.0")
    endif()
endif()

# Add hurchalla/modular_arithmetic library ###########################

add_subdirectory(lib/modular_arithmetic)

# C++ Threads library ################################################

find_package(Threads REQUIRED QUIET)

######################################################################

# Source files
set(SRC_FILES src/main.cpp src/pseudosquares_prime_sieve.cpp)

# Main executable
add_executable(pseudosquares_prime_sieve ${SRC_FILES})
target_link_libraries(pseudosquares_prime_sieve Threads::Threads primesieve::primesieve hurchalla_modular_arithmetic)
set_target_properties(pseudosquares_prime_sieve PROPERTIES CXX_STANDARD 17)

# Test executable
enable_testing()
add_executable(tests src/tests.cpp src/pseudosquares_prime_sieve.cpp)
target_link_libraries(tests primesieve::primesieve hurchalla_modular_arithmetic)
set_target_properties(tests PROPERTIES CXX_STANDARD 17)

# Register test with CTest
add_test(NAME tests COMMAND tests)
