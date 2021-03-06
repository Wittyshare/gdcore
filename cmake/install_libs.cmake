
IF(BUILD_LIB_STATIC MATCHES "true")
   ADD_LIBRARY(${APPNAME}s STATIC ${APPNAME_SRCS} )
   SET_TARGET_PROPERTIES(${APPNAME}s  PROPERTIES RELEASE_OUTPUT_NAME "${APPNAME}")
   IF(CMAKE_BUILD_TYPE MATCHES "Debug")
      SET_TARGET_PROPERTIES(${APPNAME}s  PROPERTIES DEBUG_OUTPUT_NAME   "${APPNAME}d")
   ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
ENDIF(BUILD_LIB_STATIC MATCHES "true")


IF(BUILD_LIB_SHARED MATCHES "true")
   ADD_LIBRARY(${APPNAME} SHARED ${APPNAME_SRCS} )
   SET_TARGET_PROPERTIES(${APPNAME}  PROPERTIES VERSION 1.0.1 SOVERSION 1 RELEASE_OUTPUT_NAME "${APPNAME}")
   IF(CMAKE_BUILD_TYPE MATCHES "Debug")
      SET_TARGET_PROPERTIES(${APPNAME}  PROPERTIES VERSION 1.0.1 SOVERSION 1 DEBUG_OUTPUT_NAME   "${APPNAME}d")
   ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
ENDIF(BUILD_LIB_SHARED MATCHES "true")

IF(BUILD_LIB_STATIC MATCHES "true")
   INSTALL(TARGETS ${APPNAME}s    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF(BUILD_LIB_STATIC MATCHES "true")

IF(BUILD_LIB_SHARED MATCHES "true")
   INSTALL(TARGETS ${APPNAME}     LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF(BUILD_LIB_SHARED MATCHES "true")

INSTALL(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${APPNAME} FILES_MATCHING PATTERN "*.h")

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/install_resources.cmake)

