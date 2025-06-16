# CopyWinPlatformPlugin.cmake
# This script copies the Qt Windows platform plugin (qwindows.dll) to the build directory.

# Get the Qt plugins directory
execute_process(
    COMMAND qmake -query QT_INSTALL_PLUGINS
    OUTPUT_VARIABLE QT_PLUGINS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Ensure the plugin directory exists
set(QT_PLATFORM_PLUGIN_DIR "${QT_PLUGINS_DIR}/platforms")
set(QT_WINDOWS_PLUGIN "${QT_PLATFORM_PLUGIN_DIR}/qwindows.dll")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/platforms")

# Copy the plugin
add_custom_command(
    OUTPUT "${DESTINATION_DIR}/qwindows.dll"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTINATION_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_WINDOWS_PLUGIN} ${DESTINATION_DIR}
    DEPENDS ${QT_WINDOWS_PLUGIN}
    COMMENT "Copying Qt Windows platform plugin to ${DESTINATION_DIR}"
)

# Create a target to execute the copy command
add_custom_target(copy_qt_plugin ALL DEPENDS "${DESTINATION_DIR}/qwindows.dll")