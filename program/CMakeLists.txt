# Enable Qt’s automatic features for this sub-dir only
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt5 Widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Program sources
file(GLOB_RECURSE PROGRAM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(LightningSearch ${PROGRAM_SOURCES})

# Private includes for the GUI itself
target_include_directories(LightningSearch PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link to Qt and the static lib
target_link_libraries(LightningSearch
    Qt5::Widgets
    LightningUtil
)

# Windows only: copy Qt runtime
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    include(${CMAKE_SOURCE_DIR}/cmake/CopyWinPlatformPlugin.cmake)
    add_dependencies(LightningSearch copy_qt_plugin)

    add_custom_command(
        TARGET LightningSearch POST_BUILD
        COMMAND ${CMAKE_COMMAND}
                -DEXEC_PATH=$<TARGET_FILE:LightningSearch>
                -DDEST_DIR=$<TARGET_FILE_DIR:LightningSearch>
                -P ${CMAKE_SOURCE_DIR}/cmake/CopyQtDependencies.cmake
        COMMENT "Copying Qt DLLs..."
    )
endif()
