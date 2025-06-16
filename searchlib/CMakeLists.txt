# Collect library sources
file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Static library target
add_library(LightningUtil STATIC ${LIB_SOURCES})

# Expose its public headers
target_include_directories(LightningUtil PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
