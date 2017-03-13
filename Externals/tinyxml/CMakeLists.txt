SET (SOURCES_XML
  tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp
)

SET (HEADERS_XML
  tinystr.h  tinyxmlCromeUtilities.h	tinyxml.h
)

add_library(tinyxml SHARED  ${SOURCES_XML} ${HEADERS_XML} ) 
INSTALL(TARGETS tinyxml DESTINATION lib)
install (FILES ${HEADERS_XML} DESTINATION include/ext)
