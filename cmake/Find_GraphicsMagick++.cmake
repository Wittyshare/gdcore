#- Find vmime includes and library
 #
 # This module defines
 #  GM++_INCLUDE_DIR
 #  GM++_LIBRARIES, the libraries to link against to use GM.
 #  GM++_LIB_DIR, the location of the libraries
 #  GM++_FOUND, If false, do not try to use GM
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
 IF (GM++_LIBRARIES AND GM++_INCLUDE_DIR)
    SET(GM++_FIND_QUIETLY TRUE) # Already in cache, be silent
 ENDIF (GM++_LIBRARIES AND GM++_INCLUDE_DIR)
 
 FIND_PATH(GM++_INCLUDE_DIR Magick++.h
     /usr/include
     /usr/local/include
     /usr/local/GraphicsMagick/include/GraphicsMagick
     /usr/include/GraphicsMagick
 )
 
 FIND_LIBRARY(GM++_LIBRARY NAMES GraphicsMagick++ PATHS
     /usr/lib
     /usr/local/lib
     /usr/local/GraphicsMagick/lib
 )
 
 # Copy the results to the output variables.
 IF (GM++_INCLUDE_DIR AND GM++_LIBRARY)
         SET(GM++_FOUND 1)
         SET(GM++_LIBRARIES ${GM++_LIBRARY})
         SET(GM++_INCLUDE_DIRS ${GM++_INCLUDE_DIR})
         ADD_DEFINITIONS(-DGD_HAVE_GM)
         
         MESSAGE(STATUS "Found these GraphicksMagick libs: ${GM++_LIBRARIES}")
         MESSAGE(STATUS "Found these GraphicksMagick incs: ${GM++_INCLUDE_DIRS}")
         
 ELSE (GM++_INCLUDE_DIR AND GM++_LIBRARY)
         MESSAGE(STATUS "Cannot Found GraphicksMagick libs: ${GM++_LIBRARY}")
         MESSAGE(STATUS "Cannot Found GraphicksMagick incs: ${GM++_INCLUDE_DIR}")
         SET(GM++_FOUND 0)
         SET(GM++_LIBRARIES)
         SET(GM++_INCLUDE_DIRS)
 ENDIF (GM++_INCLUDE_DIR AND GM++_LIBRARY)
 
 # Report the results.
 IF (NOT GM++_FOUND)
         SET(GM++_DIR_MESSAGE "GraphicksMagick was not found. Make sure GM++_LIBRARY and GM++_INCLUDE_DIR are set.")
         IF (NOT GM++_FIND_QUIETLY)
                 MESSAGE(STATUS "${GM++_DIR_MESSAGE}")
         ELSE (NOT GM++_FIND_QUIETLY)
                 IF (GM++_FIND_REQUIRED)
                         MESSAGE(FATAL_ERROR "${GM++_DIR_MESSAGE}")
                 ENDIF (GM++_FIND_REQUIRED)
         ENDIF (NOT GM++_FIND_QUIETLY)
 ENDIF (NOT GM++_FOUND)

 MARK_AS_ADVANCED(
     GM++_INCLUDE_DIRS
     GM++_LIBRARIES
 )
