cmake_minimum_required(VERSION 2.8.5)

# Cette librairie contient uniquement des fonctions sans interfaces.
# pour support de std::tr1
# http://www.johndcook.com/cpp_regex.html
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=gnu++11")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=gnu++11")

SET ( APPNAME_SRCS 
      src/gdCore.cpp
      src/gdCoreSql.cpp
      src/gdPdf.c
      src/gdXml.cpp
      src/gdLdap.cpp
      src/gdBarcode.c
      src/gdSpreadSheet.c
      src/gdImage.cpp
      src/bdAsterisk/AbstractAnswer.cpp
      src/bdAsterisk/Answer.cpp
      src/bdAsterisk/Conference.cpp
      src/bdAsterisk/ConnectionHandler.cpp
      src/bdAsterisk/IpAddress.cpp
      src/bdAsterisk/MeetMeListAnswer.cpp
      src/bdAsterisk/MeetMeList.cpp
      src/bdAsterisk/MeetMeMuteUnMute.cpp
      src/bdAsterisk/MeetMeUser.cpp
      src/bdAsterisk/Message.cpp
      src/bdAsterisk/OriginateCall.cpp
      src/bdAsterisk/TcpSocket.cpp
    )
INCLUDE(${CMAKE_ROOT}/Modules/FindLibXml2.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/WtFindBoost-cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/WtFindHaru.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/WtFindPostgresql.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindLdap.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindGnumeric.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/Find_GraphicsMagick++.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindCurl.cmake)

IF(NOT LIBXML2_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs xml2 libraries... Abord.")
 ELSE(NOT LIBXML2_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${LIBXML2_LIBRARIES})
  MESSAGE(STATUS "Found xml2 at  ${LIBXML2_LIBRARIES}")
ENDIF(NOT LIBXML2_FOUND)

IF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs boost libraries... Abord.")
ELSE(NOT Boost_FOUND)
  MESSAGE(STATUS "** TODO : which library name and mt or not mt ??? archive = boost_serialization, algoritm = header ou boost_regex, filesystem = boost_filesystem, lexical_cast = header")
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${Boost_LIBRARIES})
ENDIF(NOT Boost_FOUND)

IF(NOT HARU_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs haru libraries... Abord.")
 ELSE(NOT HARU_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${HARU_LIBRARIES})
ENDIF(NOT HARU_FOUND)

IF(NOT POSTGRES_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs postgres libraries... Abord.")
 ELSE(NOT POSTGRES_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${POSTGRES_LIBRARIES})
ENDIF(NOT POSTGRES_FOUND)

IF(NOT LDAP_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs ldap, lber libraries... Abord.")
 ELSE(NOT LDAP_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${LDAP_LIBRARIES})
ENDIF(NOT LDAP_FOUND)

IF(NOT GNUMERIC_FOUND)
  MESSAGE(STATUS "** GNumeric not available, disabling.")
 ELSE(NOT GNUMERIC_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} spreadsheet)
ENDIF(NOT GNUMERIC_FOUND)

IF(NOT GM++_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs GraphicsMagick++ libraries... Abord.")
 ELSE(NOT GM++_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${GM++_LIBRARIES})
ENDIF(NOT GM++_FOUND)

IF(NOT CURL_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs curl libraries... Abord.")
 ELSE(NOT CURL_FOUND)
  SET(GDTARGETLIBS ${GDTARGETLIBS} ${CURL_LIBRARIES})
ENDIF(NOT CURL_FOUND)

INCLUDE_DIRECTORIES(
    ${LIBXML2_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${HARU_INCLUDE_DIRS}
    ${POSTGRES_INCLUDE_DIRS}
    ${LDAP_INCLUDE_DIR}
    ${GNUMERIC_INCLUDE_DIRS}
    ${GM++_INCLUDE_DIR}
    /usr/include/libgoffice-0.8
    ${CURL_INCLUDE_DIR}
)

INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/install_libs.cmake)

TARGET_LINK_LIBRARIES(
    gdcore ${GDTARGETLIBS}
)

