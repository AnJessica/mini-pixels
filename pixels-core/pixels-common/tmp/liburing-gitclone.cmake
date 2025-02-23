
if(NOT "/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitinfo.txt" IS_NEWER_THAN "/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/nika/mini-pixels/pixels-core/pixels-common/liburing"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/nika/mini-pixels/pixels-core/pixels-common/liburing'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/axboe/liburing" "liburing"
    WORKING_DIRECTORY "/home/nika/mini-pixels/pixels-core/pixels-common"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/axboe/liburing'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout liburing-2.2 --
  WORKING_DIRECTORY "/home/nika/mini-pixels/pixels-core/pixels-common/liburing"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'liburing-2.2'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/nika/mini-pixels/pixels-core/pixels-common/liburing"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/nika/mini-pixels/pixels-core/pixels-common/liburing'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitinfo.txt"
    "/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/nika/mini-pixels/pixels-core/pixels-common/src/liburing-stamp/liburing-gitclone-lastrun.txt'")
endif()

