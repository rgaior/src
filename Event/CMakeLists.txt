INCLUDE_DIRECTORIES(${DETECTOR_INCLUDE_DIR}
                    ${EVENT_INCLUDE_DIR}
                    ${UTILITIES_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (SOURCES_EV
  src/Event.cc
  src/EventFile.cc
  src/Shower.cc
  src/ArrayData.cc
  src/StationData.cc
  src/SimData.cc
  src/AntennaData.cc
  src/ChannelData.cc
)

SET (HEADERS_EV
  src/Event.h
  src/EventFile.h
  src/Shower.h
  src/ArrayData.h
  src/StationData.h
  src/SimData.h
  src/AntennaData.h
  src/ChannelData.h
  src/KGBits.h
)

export_absolute_paths(EVENT_SOURCES ${SOURCES_EV})

# ROOT DICTIONARY #############################################################
ROOT_GENERATE_DICTIONARY (MWEvent
  LINKDEF src/EventLinkDef.h
  INFILES ${HEADERS_EV}
  INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)

LIST (APPEND SOURCES_EV ${MWEvent_DICT_SOURCE})
LIST (APPEND HEADERS_EV ${MWEvent_DICT_HEADER})
MESSAGE (STATUS ${MWEvent_DICT_SOURCE})

###############################################################################

SET (CMAKE_INSTALL_PREFIX $ENV{PWD})

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

ADD_LIBRARY(Event SHARED ${HEADER_EV} ${SOURCES_EV})
TARGET_LINK_LIBRARIES(Event Util ${ROOT_LIBRARIES} )

install(TARGETS Event DESTINATION lib)
install(FILES ${HEADERS_EV} DESTINATION include/evt)
