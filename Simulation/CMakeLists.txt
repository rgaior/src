SET (INCFILES
  ${UTILITIES_INCLUDE_DIR} 
  ${DETECTOR_INCLUDE_DIR} 
  ${EVENT_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TINYXML_INCLUDE_DIR} 
  ${FFTW3_INCLUDE_DIR}
)


INCLUDE_DIRECTORIES(${INCFILES})


SET (SOURCES_SIM
  src/GHShowerGenerator.cc
  src/GHShowerParameterization.cc
  src/TraceSimulator.cc
  src/BackgroundSimulator.cc
  src/ElectronicsSimulator.cc
  src/KGSimulator.cc
)

SET (HEADERS_SIM
  src/GHShowerGenerator.h
  src/GHShowerParameterization.h
  src/TraceSimulator.h
  src/BackgroundSimulator.h
  src/ElectronicsSimulator.h
  src/KGSimulator.h
)

SET (XML_SIM
  xml/GHShowerGenerator.xml
  xml/TraceSimulator.xml 
  xml/ElectronicsSimulator.xml
)

export_absolute_paths(SIMULATION_SOURCES ${SOURCES_SIM})

#################################

SET ( CMAKE_INSTALL_PREFIX $ENV{PWD})
#MESSAGE (STATUS "MWSimulation will be installed in ${CMAKE_INSTALL_PREFIX}")


################### PART TO ADD THE LIBRARY TO THE SYSTEM
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

FIND_LIBRARY(ROOT_XMLIO_LIBRARY XMLIO ${ROOT_LIBDIR}) 

ADD_LIBRARY (Sim SHARED ${HEADER_SIM} ${SOURCES_SIM})

TARGET_LINK_LIBRARIES (Sim Event Detector Util tinyxml ${ROOT_LIBRARIES} ${ROOT_GUI_LIBRARIES} ${ROOT_XMLIO_LIBRARY} ${FFTW3_LIBRARIES})

install (TARGETS Sim DESTINATION lib)
install (FILES ${XML_SIM} DESTINATION config)
install (FILES ${HEADERS_SIM} DESTINATION include/sim)
