cmake_minimum_required(VERSION 3.5)
project(LightningSearch LANGUAGES CXX)

# Global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional unit-tests
option(BUILD_TESTING "Build unit tests" ON)

# Option for whether the entire GUI program
# should be built, or just library+unit tests.
# If only headless unit testing is desired,
# for example for CI, this can be turned off.
option(BUILD_GUI "Build main program" ON)

# Add component directories
add_subdirectory(searchlib)

if(BUILD_GUI)
    add_subdirectory(program)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
