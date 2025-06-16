# CopyQtDependencies.cmake
# This script uses ldd to determine dependencies of the built
# executable and copies them from /mingw64/bin to the project build directory.

if(NOT DEFINED EXEC_PATH)
  message(FATAL_ERROR "EXEC_PATH is not defined")
endif()
if(NOT DEFINED DEST_DIR)
  message(FATAL_ERROR "DEST_DIR is not defined")
endif()

# Run "ldd" on the executable
execute_process(
    COMMAND ldd "${EXEC_PATH}"
    OUTPUT_VARIABLE _ldd_output
    ERROR_VARIABLE  _ldd_error
    RESULT_VARIABLE _ldd_result
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(_ldd_result)
  message(WARNING "ldd reported an error: ${_ldd_error}")
endif()

# Extract the DLL paths from each line of ldd’s output.
string(REGEX MATCHALL "=> [^ ]+ \\(" _temp_matches "${_ldd_output}")

set(_dll_matches "")
foreach(_m IN LISTS _temp_matches)
  # This strips off "=> " and everything after the space before "(":
  # so "=> /ucrt64/bin/libicuin72.dll (" => "/ucrt64/bin/libicuin72.dll"
  string(REGEX REPLACE ".*=> ([^ ]+) \\(.*" "\\1" _clean_path "${_m}")

  # Now _clean_path should be "/ucrt64/bin/libicuin72.dll"
  list(APPEND _dll_matches "${_clean_path}")
endforeach()

# For each matched DLL, check if it's from /mingw64/bin and copy it.
foreach(_dll_line IN LISTS _dll_matches)
  # _dll_line should be something like "/mingw64/bin/libglib-2.0-0.dll"
  # Trim leading/trailing whitespace just in case:
  string(STRIP "${_dll_line}" _dll_path)

  # Check if the line is from /mingw64/bin
  if(_dll_path MATCHES "/mingw64/bin/(.+\\.dll)$")
    message(STATUS "Copying needed DLL: ${_dll_path}")

    execute_process(
      COMMAND cp "${_dll_path}" "${DEST_DIR}/"
    )
  else()
    message(STATUS "Ignoring non-mingw64 DLL or system path: ${_dll_path}")
  endif()
endforeach()
