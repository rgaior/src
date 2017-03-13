# Install script for directory: /Users/ImenAlSamarai/Documents/mbr/Simulation

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSim.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSim.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSim.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ImenAlSamarai/Documents/mbr/Install/Examples/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSim.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSim.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/config" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/xml/GHShowerGenerator.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/xml/TraceSimulator.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/xml/ElectronicsSimulator.xml"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sim" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/GHShowerGenerator.h"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/GHShowerParameterization.h"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/TraceSimulator.h"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/BackgroundSimulator.h"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/ElectronicsSimulator.h"
    "/Users/ImenAlSamarai/Documents/mbr/Simulation/src/KGSimulator.h"
    )
endif()

