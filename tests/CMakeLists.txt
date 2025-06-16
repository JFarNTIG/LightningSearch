# --- Fetch GoogleTest once ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # MSVC runtime harmony
FetchContent_MakeAvailable(googletest)

# Test sources
file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(LightningTests ${TEST_SOURCES})

target_link_libraries(LightningTests
    PRIVATE
        gtest_main
        LightningUtil
)

# Auto-discover tests
include(GoogleTest)
gtest_discover_tests(LightningTests)
