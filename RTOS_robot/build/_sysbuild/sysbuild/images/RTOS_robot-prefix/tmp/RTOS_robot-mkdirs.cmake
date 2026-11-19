# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot")
  file(MAKE_DIRECTORY "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot")
endif()
file(MAKE_DIRECTORY
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/RTOS_robot"
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix"
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/tmp"
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/src/RTOS_robot-stamp"
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/src"
  "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/src/RTOS_robot-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/src/RTOS_robot-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/manes/shittia/RTOS_kurssi/RTOS_robot/build/_sysbuild/sysbuild/images/RTOS_robot-prefix/src/RTOS_robot-stamp${cfgdir}") # cfgdir has leading slash
endif()
