# Install script for directory: /Users/ImenAlSamarai/Documents/mbr/Event

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/ImenAlSamarai/Documents/mbr/Install/Examples")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEvent.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEvent.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEvent.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ImenAlSamarai/Documents/mbr/Install/Examples/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEvent.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEvent.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/evt" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/Event.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/EventFile.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/Shower.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/ArrayData.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/StationData.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/SimData.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/AntennaData.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/ChannelData.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/src/KGBits.h"
    "/Users/ImenAlSamarai/Documents/mbr/Event/MWEventDictionary.h"
    )
endif()

