# Install script for directory: /Users/ImenAlSamarai/Documents/mbr/Examples

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
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/testEventFile")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/testEventFile")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testEventFile" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testEventFile")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testEventFile")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testEventFile")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/convertRawData")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/convertRawData")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/convertRawData" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/convertRawData")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/convertRawData")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/convertRawData")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/selectKGCandidates")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/selectKGCandidates")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/selectKGCandidates" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/selectKGCandidates")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/selectKGCandidates")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/selectKGCandidates")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/testAtmosphere")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/testAtmosphere")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testAtmosphere" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testAtmosphere")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testAtmosphere")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testAtmosphere")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/analyzeSim_1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/analyzeSim_1")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/analyzeSim_1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/analyzeSim_1")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/analyzeSim_1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/analyzeSim_1")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/mwSimulation_1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/mwSimulation_1")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/mwSimulation_1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/mwSimulation_1")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/mwSimulation_1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/mwSimulation_1")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/testFFTW3")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/testFFTW3")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testFFTW3" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testFFTW3")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testFFTW3")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/stackEvents")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/stackEvents")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/stackEvents" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/stackEvents")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/stackEvents")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/stackEvents")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvent")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/simulateEvent")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvent" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvent")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvent")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvent")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/searchEvents")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/searchEvents")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchEvents" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchEvents")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchEvents")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchEvents")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/checkTiming")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/checkTiming")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/checkTiming" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/checkTiming")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/checkTiming")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/checkTiming")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/addCorrectedTriggerTime")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/addCorrectedTriggerTime")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/addCorrectedTriggerTime" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/addCorrectedTriggerTime")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/addCorrectedTriggerTime")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/addCorrectedTriggerTime")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvents")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/simulateEvents")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvents" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvents")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvents")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/simulateEvents")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/testTsky")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/testTsky")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testTsky" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testTsky")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testTsky")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testTsky")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/RunSensitivity")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/RunSensitivity")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/RunSensitivity" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/RunSensitivity")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/RunSensitivity")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/RunSensitivity")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/Data_FFT")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/Data_FFT")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/Data_FFT" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/Data_FFT")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/Data_FFT")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/Data_FFT")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/MakeRandomTraces")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/MakeRandomTraces")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/MakeRandomTraces" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/MakeRandomTraces")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/MakeRandomTraces")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/MakeRandomTraces")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/SimData")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/SimData")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/SimData" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/SimData")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/SimData")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/SimData")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/searchGigas")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/searchGigas")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchGigas" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchGigas")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchGigas")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/searchGigas")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/AddBackground")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/AddBackground")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/AddBackground" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/AddBackground")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/AddBackground")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/AddBackground")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/ImenAlSamarai/Documents/mbr/bin/testwindows")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/ImenAlSamarai/Documents/mbr/bin" TYPE EXECUTABLE FILES "/Users/ImenAlSamarai/Documents/mbr/Examples/testwindows")
  if(EXISTS "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testwindows" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testwindows")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib" "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Simulation/libSim.dylib" "libSim.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testwindows")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/ImenAlSamarai/Documents/mbr/bin/testwindows")
    endif()
  endif()
endif()

