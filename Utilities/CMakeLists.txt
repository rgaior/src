SET (INCFILES 
  ${UTILITIES_INCLUDE_DIR}
  ${TINYXML_INCLUDE_DIR} 
  ${GTEST_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES(${INCFILES})

SET (SOURCES_UTL
  src/Configuration.cc
  src/PhysicsFunctions.cc
  src/MathFunctions.cc
  src/KascadeEnergyConverter.cc
  src/RandomGenerator.cc
  src/TimeStamp.cc
  src/TimedClass.cc
)

SET (HEADERS_UTL
  src/PhysicsFunctions.h
  src/MathFunctions.h
  src/KascadeEnergyConverter.h
  src/Configuration.h
  src/TimeStamp.h 
  src/RandomGenerator.h
  src/Singleton.h
  src/AugerUnits.h
  src/MathConstants.h
  src/PhysicalConstants.h
  src/BitTools.h
  src/TimedClass.h
)

export_absolute_paths(UTILITIES_SOURCES ${SOURCES_UTL})

################### PART TO ADD THE LIBRARY TO THE SYSTEM
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

ADD_LIBRARY (Util SHARED ${HEADER_UTL} ${SOURCES_UTL})
#TARGET_LINK_LIBRARIES (Util ${ROOT_LIBRARIES})

target_link_libraries(Util ${ROOT_LIBRARIES})

install (TARGETS Util DESTINATION lib)
install (FILES ${HEADERS_UTL} DESTINATION include/utl)
install (FILES KascadeEnergyTable.txt DESTINATION config)


################### UNIT TESTS
# testMathFunctions
#add_executable(testMathFunctions tests/MathFunctionsTest.cc)
#target_link_libraries(testMathFunctions Util tinyxml ${ROOT_LIBRARIES}
#					gtest_main)

#install(TARGETS testMathFunctions DESTINATION bin/tests)

# add_test(NAME testMathFunctions
#          WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/tests
#          COMMAND testMathFunctions)
