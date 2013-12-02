#- Find vmime includes and library
 #
 # This module defines
 #  GNUMERIC_INCLUDE_DIR
 #  GNUMERIC_LIBRARIES, the libraries to link against to use GM.
 #  GNUMERIC_LIB_DIR, the location of the libraries
 #  GNUMERIC_FOUND, If false, do not try to use GM
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 

 IF (GNUMERIC_LIBRARIES AND GNUMERIC_INCLUDE_DIR)
    SET(GNUMERIC_FIND_QUIETLY TRUE) # Already in cache, be silent
 ENDIF (GNUMERIC_LIBRARIES AND GNUMERIC_INCLUDE_DIR)
 
IF ( DEFINED GD_HAVE_GNUMERIC AND GD_HAVE_GNUMERIC EQUAL TRUE )
 INCLUDE(${CMAKE_ROOT}/Modules/FindGTK2.cmake)
ENDIF ( DEFINED GD_HAVE_GNUMERIC AND GD_HAVE_GNUMERIC EQUAL TRUE )

FIND_PACKAGE(PkgConfig)

FIND_PATH(GLIB2_INCLUDE_DIR glib.h
          HINTS ${GLIB_INCLUDE_DIRS})

FIND_PATH(GLIB2_CONFIG_INCLUDE_DIR glibconfig.h
          HINTS ${GLIB_INCLUDE_DIRS})
 
FIND_LIBRARY(GLIB2_LIBRARY NAMES glib-2.0 glib
             HINTS ${GLIB_LIBDIR})

FIND_PATH(GNUMERIC_INCLUDE_DIR gnumeric.h
    /usr/local/gnumeric/include/libspreadsheet-1.10/spreadsheet
)
 
FIND_LIBRARY(GOFFICE_LIBRARY NAMES goffice-0.8 PATHS
     /usr/local/gnumeric/lib
     /usr/lib/
)

FIND_LIBRARY(GSF_LIBRARY NAMES gsf-1 PATHS
    /usr/local/gnumeric/lib
    /usr/lib/
    /usr/local/lib
)

FIND_LIBRARY(GNUMERIC_LIBRARY NAMES spreadsheet PATHS
     /usr/local/gnumeric/lib
     /usr/lib/
     /usr/local/lib
)
 
 # Copy the results to the output variables.
 IF (GNUMERIC_INCLUDE_DIR AND GNUMERIC_LIBRARY)
         SET(GNUMERIC_FOUND 1)
         ADD_DEFINITIONS(-DGD_HAVE_GNUMERIC)
         SET(GNUMERIC_LIBRARIES ${GNUMERIC_LIBRARY} ${GOFFICE_LIBRARY} ${GSF_LIBRARY} ${GLIB_LIBDIR} ${GTK2_LIBRARIES} )
         SET(GNUMERIC_INCLUDE_DIRS ${GNUMERIC_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} ${GTK2_INCLUDE_DIRS} /usr/local/gnumeric/include/libgoffice-0.8 /usr/local/gnumeric/include/libgsf-1)
         MESSAGE(STATUS "Found these gnumeric libs: ${GNUMERIC_LIBRARIES}")
 ELSE (GNUMERIC_INCLUDE_DIR AND GNUMERIC_LIBRARY)
         SET(GNUMERIC_FOUND 0)
         SET(GNUMERIC_LIBRARIES)
         SET(GNUMERIC_INCLUDE_DIRS)
 ENDIF (GNUMERIC_INCLUDE_DIR AND GNUMERIC_LIBRARY)
 
 # Report the results.
 IF (NOT GNUMERIC_FOUND)
         SET(GNUMERIC_DIR_MESSAGE "gnumeric was not found. Make sure GNUMERIC_LIBRARY and GNUMERIC_INCLUDE_DIR are set.")
         IF (NOT GNUMERIC_FIND_QUIETLY)
                 MESSAGE(STATUS "${GNUMERIC_DIR_MESSAGE}")
         ELSE (NOT GNUMERIC_FIND_QUIETLY)
                 IF (GNUMERIC_FIND_REQUIRED)
                         MESSAGE(FATAL_ERROR "${GNUMERIC_DIR_MESSAGE}")
                 ENDIF (GNUMERIC_FIND_REQUIRED)
         ENDIF (NOT GNUMERIC_FIND_QUIETLY)
 ENDIF (NOT GNUMERIC_FOUND)
 
 
 MARK_AS_ADVANCED(
     GNUMERIC_INCLUDE_DIRS
     GNUMERIC_LIBRARIES
 )

