################################################################################
# Module to find GRASP                                                         #
#                                                                              #
# This sets the following variables:                                           #
#   - GRASP_FOUND                                                              #
#   - GRASP_BIN                                                                #
#                                                                              #
################################################################################

SET (HAVE_GRASP 0)

# Search directories for the GRAS executable.
SET (DIRECTORIES
  $ENV{GRASPROOT}
)

FIND_PROGRAM ( GRASP_BIN grasp9-se PATHS ${DIRECTORIES} PATH_SUFFIXES bin)

IF (GRASP_BIN)
  SET (GRASP_FOUND TRUE)
  SET (HAVE_GRASP 1)
ENDIF (GRASP_BIN)


IF (GRASP_FOUND)
  IF (NOT GRASP_FIND_QUIETLY)
    MESSAGE (STATUS "Found GRASP: ${GRASP_BIN}")
  ENDIF (NOT GRASP_FIND_QUIETLY)
ELSE (GRASP_FOUND)
  IF (GRASP_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR "Could not find GRASP")
  ELSE (GRASP_FIND_REQUIRED)
    IF (NOT GRASP_FIND_QUIETLY)
      MESSAGE (STATUS "GRASP was not found in the system")
    ENDIF (NOT GRASP_FIND_QUIETLY)
   ENDIF (GRASP_FIND_REQUIRED)
ENDIF (GRASP_FOUND)

