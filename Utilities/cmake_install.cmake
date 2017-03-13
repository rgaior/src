# Install script for directory: /Users/ImenAlSamarai/Documents/mbr/Utilities

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/ImenAlSamarai/Documents/mbr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtil.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtil.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libUtil.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtil.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ImenAlSamarai/Documents/mbr/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtil.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtil.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/utl" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/PhysicsFunctions.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/MathFunctions.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/KascadeEnergyConverter.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/Configuration.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/TimeStamp.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/RandomGenerator.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/Singleton.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/AugerUnits.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/MathConstants.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/PhysicalConstants.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/BitTools.h"
    "/Users/ImenAlSamarai/Documents/mbr/Utilities/src/TimedClass.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/config" TYPE FILE FILES "/Users/ImenAlSamarai/Documents/mbr/Utilities/KascadeEnergyTable.txt")
endif()

