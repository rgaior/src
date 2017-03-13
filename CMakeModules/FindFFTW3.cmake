################################################################################
# Module to find FFTW3                                                         #
#                                                                              #
# This sets the following variables:                                           #
#   - FFTW3_FOUND                                                              #
#   - FFTW3_DEFINED                                                            #
#   - FFTW3_LIBRARIES                                                          #
#   - FFTW_CPPFLAGS                                                            #
#   - FFTW_LDFLAGS                                                             #
#   - FFTW3_INCLUDE_DIR                                                        #
################################################################################

SET (HAVE_FFTW3 0)

# Search directories for the FFTW3 module.
SET (DIRECTORIES
  $ENV{FFTW3_PATH}
  $ENV{FFTW3ROOT}
)

FIND_PATH (FFTW3_INCLUDE_DIR fftw3.h PATHS ${DIRECTORIES} PATH_SUFFIXES include)
FIND_LIBRARY (FFTW3_LIBRARIES fftw3 PATHS ${DIRECTORIES} PATH_SUFFIXES lib lib64)

IF (FFTW3_LIBRARIES AND FFTW3_INCLUDE_DIR)
  SET (FFTW3_FOUND TRUE)
  SET (HAVE_FFTW3 1)
  SET (FFTW3_LDFLAGS ${FFTW_LIBRARIES})
  SET (FFTW3_CPPFLAGS "-I${FFTW3_INCLUDE_DIR}")
ENDIF (FFTW3_LIBRARIES AND FFTW3_INCLUDE_DIR)

IF (FFTW3_FOUND)
  IF (NOT FFTW3_FIND_QUIETLY)
    MESSAGE (STATUS "Found FFTW3: ${FFTW3_LIBRARIES}")
    MESSAGE (STATUS "Found FFTW3: ${FFTW3_INCLUDE_DIR}")
  ENDIF (NOT FFTW3_FIND_QUIETLY)
ELSE (FFTW3_FOUND)
  IF (FFTW3_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR "Could not find FFTW3")
  ELSE (FFTW3_FIND_REQUIRED)
    IF (NOT FFTW3_FIND_QUIETLY)
      MESSAGE (STATUS "FFTW3 was not found in the system")
    ENDIF (NOT FFTW3_FIND_QUIETLY)
   ENDIF (FFTW3_FIND_REQUIRED)
ENDIF (FFTW3_FOUND)
