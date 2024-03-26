# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4.5/components/bootloader/subproject"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix/tmp"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix/src"
  "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Espressif/frameworks/dev/bangDienIot/firmware/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
