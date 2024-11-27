# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-src"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-build"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/tmp"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/src"
  "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Code/C/Webstockprj/Webstockprj/client/cmake-build-debug-qt/_deps/curl-subbuild/curl-populate-prefix/src/curl-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
