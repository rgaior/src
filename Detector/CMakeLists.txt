SET (INCFILES
  ${DETECTOR_INCLUDE_DIR}
  ${EVENT_INCLUDE_DIR}
  ${UTILITIES_INCLUDE_DIR} 
  ${TINYXML_INCLUDE_DIR} 
  ${GTEST_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES(${INCFILES})

SET (SOURCES_DET
  src/Atmosphere.cc
  src/Array.cc
  src/Station.cc
  src/Channel.cc
  src/ChannelGain.cc
  src/ChannelMap.cc
  src/Detector.cc
  src/Antenna.cc
  src/DetectorSetup.cc
)

SET (HEADERS_DET
  src/Atmosphere.h
  src/Array.h
  src/Station.h
  src/Channel.h
  src/ChannelGain.h
  src/ChannelMap.h
  src/Detector.h
  src/Antenna.h
  src/DetectorSetup.h
)

SET (XML_DET
  xml/ArrayModels.xml
  xml/ChannelMap_12ch.xml
  xml/ChannelMap-test.xml
  xml/ChannelModels.xml
  xml/ChannelMap-110511.xml
  xml/ChannelMap-110606.xml
  xml/ChannelMap-111024.xml
  xml/ChannelMap-120404.xml
  xml/ChannelMap-120516.xml
  xml/ChannelMap-120702.xml
  xml/ChannelMap-120712.xml
  xml/ChannelMap-120720.xml
  xml/ChannelMap-120724.xml
  xml/ChannelMap-120801.xml
  xml/Config-110511.xml
  xml/Config-110606.xml
  xml/Config-111024.xml
  xml/Config-120404.xml
  xml/Config-120516.xml
  xml/Config-120615.xml
  xml/Config-120702.xml
  xml/Config-120712.xml
  xml/Config-120720.xml
  xml/Config-120724.xml
  xml/Config-120801.xml
  xml/CROME-setup1.xml
  xml/DigitizerModels.xml
  xml/DishModels.xml
  xml/ElectronicsChainModels.xml
  xml/GainPattern-grasp-central_feed.xml
  xml/GainPattern-grasp-corner_feed.xml
  xml/GainPattern-grasp-side_feed.xml
  xml/MicrowaveDetector.xml
  xml/MicrowaveDetector_8ch.xml
  xml/MicrowaveDetector_08ch.xml
  xml/MicrowaveDetector_12ch.xml
  xml/MicrowaveDetector-110511.xml
  xml/MicrowaveDetector-111024.xml
  xml/MicrowaveDetector-120404.xml
  xml/MicrowaveDetector-120516.xml
  xml/MicrowaveDetector-120615.xml
  xml/MicrowaveDetector-120702.xml
  xml/ReceiverModels.xml
  xml/TimeOffsetTable.xml
)

export_absolute_paths(DETECTOR_SOURCES ${SOURCES_DET})

################### GENERATE XML CONFIGURATIONS WITH PROPER PATHS
add_custom_target(StandardConfig_12ch ALL)
add_custom_command(
    SOURCE    xml/StandardConfig_12ch.xml.in
    COMMAND   sed -e 's+@XMLCONFIGDIR@+'${CMAKE_INSTALL_PREFIX}'/config+' ${CMAKE_CURRENT_SOURCE_DIR}/xml/StandardConfig_12ch.xml.in > ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_12ch.xml
    TARGET    StandardConfig_12ch
    OUTPUTS   ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_12ch.xml
)

add_custom_target(StandardConfig_08ch ALL)
add_custom_command(
    SOURCE    xml/StandardConfig_08ch.xml.in
    COMMAND   sed -e 's+@XMLCONFIGDIR@+'${CMAKE_INSTALL_PREFIX}'/config+' ${CMAKE_CURRENT_SOURCE_DIR}/xml/StandardConfig_08ch.xml.in > ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_08ch.xml
    TARGET    StandardConfig_08ch
    OUTPUTS   ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_08ch.xml
)


add_custom_target(Config-Test ALL)
add_custom_command(
    SOURCE    xml/Config-Test.xml.in
    COMMAND   sed -e 's+@XMLCONFIGDIR@+'${CMAKE_INSTALL_PREFIX}'/config+' ${CMAKE_CURRENT_SOURCE_DIR}/xml/Config-Test.xml.in > ${CMAKE_CURRENT_BINARY_DIR}/Config-Test.xml
    TARGET    Config-Test
    OUTPUTS   ${CMAKE_CURRENT_BINARY_DIR}/Config-Test.xml
)


add_custom_target(MicrowaveDetector-120702 ALL)
add_custom_command(
    SOURCE    xml/MicrowaveDetector-120702.xml.in
    COMMAND   sed -e 's+@XMLCONFIGDIR@+'${CMAKE_INSTALL_PREFIX}'/config+' ${CMAKE_CURRENT_SOURCE_DIR}/xml/MicrowaveDetector-120702.xml.in > ${CMAKE_CURRENT_BINARY_DIR}/MicrowaveDetector-120702.xml
    TARGET    MicrowaveDetector-120702
    OUTPUTS   ${CMAKE_CURRENT_BINARY_DIR}/MicrowaveDetector-120702.xml
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_12ch.xml
	      ${CMAKE_CURRENT_BINARY_DIR}/StandardConfig_08ch.xml
              ${CMAKE_CURRENT_BINARY_DIR}/Config-Test.xml	
              ${CMAKE_CURRENT_BINARY_DIR}/MicrowaveDetector-120702.xml	
              DESTINATION config)



################### ROOT DICTIONARY
ROOT_GENERATE_DICTIONARY (DetectorSetup
  LINKDEF src/DetectorSetupLinkDef.h
  INFILES ${HEADERS_DET}
  INCLUDE_DIRS ${INCFILES}
)

LIST (APPEND SOURCES_DET ${DetectorSetup_DICT_SOURCE})
LIST (APPEND HEADERS_DET ${DetectorSetup_DICT_HEADER})
LIST (APPEND HEADERS_DET ${DetectorSetup_DICT_HEADER})

MESSAGE (STATUS ${Det_DICT_SOURCE})


################### PART TO ADD THE LIBRARY TO THE SYSTEM
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


ADD_LIBRARY (Detector SHARED ${HEADER_DET} ${SOURCES_DET})
TARGET_LINK_LIBRARIES (Detector Event tinyxml ${ROOT_LIBRARIES})

install (TARGETS Detector DESTINATION lib)
install (FILES ${HEADERS_DET} DESTINATION include/det)
install (FILES ${XML_DET} DESTINATION config)


################### UNIT TESTS
# testChannelMap
#add_executable(testChannelMap tests/ChannelMapTest.cc)
#target_link_libraries(testChannelMap Detector Util tinyxml gtest_main)

#install(TARGETS testChannelMap DESTINATION bin/tests)
#install(DIRECTORY tests/ChannelMapTest.in DESTINATION bin/tests
#        PATTERN ".svn" EXCLUDE)

 #add_test(NAME testChannelMap
 #         WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/tests/ChannelMapTest.in
 #         COMMAND testChannelMap)

# testChannelGain
#add_executable(testChannelGain tests/ChannelGainTest.cc)
#target_link_libraries(testChannelGain Detector Util tinyxml ${ROOT_LIBRARIES}
#				      gtest_main)

#install(TARGETS testChannelGain DESTINATION bin/tests)

# add_test(NAME testChannelGain
#          WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/tests
#          COMMAND testChannelGain)
