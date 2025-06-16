cmake_minimum_required(VERSION 3.5)
project(LightningSearch LANGUAGES CXX)

# Global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional unit-tests
option(BUILD_TESTING "Build unit tests" ON)

# Add component directories
add_subdirectory(searchlib)
add_subdirectory(program)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
