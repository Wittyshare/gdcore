#- Find vmime includes and library
 #
 # This module defines
 #  GM_INCLUDE_DIR
 #  GM_LIBRARIES, the libraries to link against to use GM.
 #  GM_LIB_DIR, the location of the libraries
 #  GM_FOUND, If false, do not try to use GM
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
 IF (GM_LIBRARIES AND GM_INCLUDE_DIR)
    SET(GM_FIND_QUIETLY TRUE) # Already in cache, be silent
 ENDIF (GM_LIBRARIES AND GM_INCLUDE_DIR)
 
 
 FIND_PATH(GM_INCLUDE_DIR magick/api.h
     /usr/include
     /usr/local/include
     /usr/local/GraphicsMagick/include/GraphicsMagick
     /usr/include/GraphicsMagick
 )
 
 FIND_LIBRARY(GM_LIBRARY NAMES GraphicsMagick PATHS
     /usr/lib
     /usr/local/lib
     /usr/local/GraphicsMagick/lib
 )
 
 # Copy the results to the output variables.
 IF (GM_INCLUDE_DIR AND GM_LIBRARY)
         SET(GM_FOUND 1)
         SET(GM_LIBRARIES ${GM_LIBRARY})
         SET(GM_INCLUDE_DIRS ${GM_INCLUDE_DIR})
         ADD_DEFINITIONS(-DGD_HAVE_GM)
         
         MESSAGE(STATUS "Found these GraphicksMagick libs: ${GM_LIBRARIES}")
         
 ELSE (GM_INCLUDE_DIR AND GM_LIBRARY)
         SET(GM_FOUND 0)
         SET(GM_LIBRARIES)
         SET(GM_INCLUDE_DIRS)
 ENDIF (GM_INCLUDE_DIR AND GM_LIBRARY)
 
 # Report the results.
 IF (NOT GM_FOUND)
         SET(GM_DIR_MESSAGE "GraphicksMagick was not found. Make sure GM_LIBRARY and GM_INCLUDE_DIR are set.")
         IF (NOT GM_FIND_QUIETLY)
                 MESSAGE(STATUS "${GM_DIR_MESSAGE}")
         ELSE (NOT GM_FIND_QUIETLY)
                 IF (GM_FIND_REQUIRED)
                         MESSAGE(FATAL_ERROR "${GM_DIR_MESSAGE}")
                 ENDIF (GM_FIND_REQUIRED)
         ENDIF (NOT GM_FIND_QUIETLY)
 ENDIF (NOT GM_FOUND)
 
 
 MARK_AS_ADVANCED(
     GM_INCLUDE_DIRS
     GM_LIBRARIES
 )
