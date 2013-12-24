#
# We set:
# - GDCORE_INCLUDE_DIR  : the gdcore include dir
# - GDCORE_INCLUDE_DIRS : the gdcore include dir + all required include
# - GDCORE_LIBRARY      : the gdcore libraries
# - GDCORE_LIBRARIES    : the gdcore libraries + all required libraries
# - GDDEBUG             : = 1 enabled else disabled
# - GDCORE_FOUND

FIND_PATH(GDCORE_INCLUDE_DIR gdcore/gdCore.h PATHS
    ${GDCORE_PREFIX}/include
)

IF( GDCORE_INCLUDE_DIR )
  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    FIND_LIBRARY(GDCORE_LIBRARY       NAMES gdcored PATHS ${GDCORE_PREFIX}/lib)
     ADD_DEFINITIONS(-DGDDEBUG=1)
   ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
     FIND_LIBRARY(GDCORE_LIBRARY       NAMES gdcore  PATHS ${GDCORE_PREFIX}/lib)
     ADD_DEFINITIONS(-DGDDEBUG=0)
   ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

   IF( GDCORE_LIBRARY )
     SET(GDCORE_FOUND TRUE)
   ELSE(GDCORE_LIBRARY)
     SET(GDCORE_FOUND FALSE)
   ENDIF(GDCORE_LIBRARY)

   IF(GDCORE_FOUND)
      MESSAGE(STATUS "Found the gdcore ${CMAKE_BUILD_TYPE} libraries at ${GDCORE_LIBRARY}")
      MESSAGE(STATUS "Found the gdcore ${CMAKE_BUILD_TYPE} headers   at ${GDCORE_INCLUDE_DIR}")
   ELSE(GDCORE_FOUND)
      MESSAGE(FATAL "Could NOT find gdcore ${CMAKE_BUILD_TYPE} libraries")
   ENDIF(GDCORE_FOUND)
ENDIF( GDCORE_INCLUDE_DIR )

IF(GDCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/FindLibXml2.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindHaru.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindPostgresql.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindLdap.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindGnumeric.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_GraphicsMagick.cmake)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindCurl.cmake)

   IF(NOT LIBXML2_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs xml2 libraries... Abord.")
   ENDIF(NOT LIBXML2_FOUND)
   
   IF(NOT Boost_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs boost libraries... Abord.")
   ENDIF(NOT Boost_FOUND)
   
   IF(NOT HARU_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs haru libraries... Abord.")
   ENDIF(NOT HARU_FOUND)
   
   IF(NOT POSTGRES_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs postgres libraries... Abord.")
   ENDIF(NOT POSTGRES_FOUND)
   
   IF(NOT LDAP_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs ldap, lber libraries... Abord.")
   ENDIF(NOT LDAP_FOUND)
   
   IF(NOT GM_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs Graphics Magick libraries... Abord.")
   ENDIF(NOT GM_FOUND)

   IF(NOT CURL_FOUND)
     MESSAGE(FATAL_ERROR "** these program needs curl libraries... Abord.")
   ENDIF(NOT CURL_FOUND)
   
   SET(GDCORE_INCLUDE_DIRS
        ${LIBXML2_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${HARU_INCLUDE_DIRS}
        ${POSTGRES_INCLUDE_DIRS}
        ${LDAP_INCLUDE_DIR}
        ${GNUMERIC_INCLUDE_DIRS}
        ${GM_INCLUDE_DIR}
        ${GDCORE_INCLUDE_DIR}
        ${CURL_INCLUDE_DIR}
   )

   SET(GDCORE_LIBRARIES
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        ${HARU_LIBRARIES}
        ${POSTGRES_LIBRARIES}
        ${LDAP_LIBRARIES}
        ${GNUMERIC_LIBRARIES}
        ${GM_LIBRARIES}
        ${GDCORE_LIBRARY}
        ${CURL_LIBRARIES}
   )
#message(STATUS "Found gdcore all : ${GDCORE_LIBRARIES}")
ENDIF(GDCORE_FOUND)

