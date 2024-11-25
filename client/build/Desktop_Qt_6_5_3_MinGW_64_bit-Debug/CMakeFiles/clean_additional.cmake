# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LoginWidget_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LoginWidget_autogen.dir\\ParseCache.txt"
  "LoginWidget_autogen"
  )
endif()
