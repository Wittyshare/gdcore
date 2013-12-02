ADD_EXECUTABLE(
               ${APPNAME}
               ${APPNAME_SRCS} 
              )
  
TARGET_LINK_LIBRARIES(
                      ${APPNAME}
                      ${APP_LIBRARIES}
                     )

IF(CMAKE_BUILD_TYPE MATCHES "Release")
   INSTALL(PROGRAMS  ${CMAKE_CURRENT_BINARY_DIR}/${APPNAME}       DESTINATION  /usr/local/lassie/bin/ CONFIGURATIONS Release)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
   INSTALL(PROGRAMS  ${CMAKE_CURRENT_BINARY_DIR}/${APPNAME}       DESTINATION  /usr/local/lassie/bin/ CONFIGURATIONS Debug    RENAME ${APPNAME}d)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

