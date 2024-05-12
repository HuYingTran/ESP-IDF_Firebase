# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4.5/components/bootloader/subproject"
  "E:/bangDienIot/firmware/build/bootloader"
  "E:/bangDienIot/firmware/build/bootloader-prefix"
  "E:/bangDienIot/firmware/build/bootloader-prefix/tmp"
  "E:/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp"
  "E:/bangDienIot/firmware/build/bootloader-prefix/src"
  "E:/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
