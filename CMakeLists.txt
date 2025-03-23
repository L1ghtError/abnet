cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(abnet)

#options
option(ENABLE_TESTS "build tests" OFF)

#gtest
if(ENABLE_TESTS)
    #includes
    ENABLE_TESTING()
    include_directories("include")
    include_directories("tests")

    project(googletest-git NONE)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory("tests")
else()
    message(FATAL_ERROR "Tests are disabled. Enable with -DENABLE_TESTS=ON.")
endif()
