# Install script for directory: /Users/ImenAlSamarai/Documents/mbr/Detector

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/config" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Detector/StandardConfig_12ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/StandardConfig_08ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/Config-Test.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/MicrowaveDetector-120702.xml"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ImenAlSamarai/Documents/mbr/Detector/libDetector.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDetector.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDetector.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libDetector.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Event/libEvent.dylib" "libEvent.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Externals/tinyxml/libtinyxml.dylib" "libtinyxml.dylib"
      -change "/Users/ImenAlSamarai/Documents/mbr/Utilities/libUtil.dylib" "libUtil.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDetector.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/ImenAlSamarai/Documents/mbr/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDetector.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libDetector.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/det" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Atmosphere.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Array.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Station.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Channel.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelGain.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/ChannelMap.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Detector.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/Antenna.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/src/DetectorSetup.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.h"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/DetectorSetupDictionary.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/config" TYPE FILE FILES
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ArrayModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap_12ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-test.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-110511.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-110606.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-111024.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120404.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120516.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120702.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120712.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120720.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120724.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ChannelMap-120801.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-110511.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-110606.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-111024.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120404.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120516.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120615.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120702.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120712.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120720.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120724.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/Config-120801.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/CROME-setup1.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/DigitizerModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/DishModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ElectronicsChainModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/GainPattern-grasp-central_feed.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/GainPattern-grasp-corner_feed.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/GainPattern-grasp-side_feed.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector_8ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector_08ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector_12ch.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-110511.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-111024.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-120404.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-120516.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-120615.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/MicrowaveDetector-120702.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/ReceiverModels.xml"
    "/Users/ImenAlSamarai/Documents/mbr/Detector/xml/TimeOffsetTable.xml"
    )
endif()

