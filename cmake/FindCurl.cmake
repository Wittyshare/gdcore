# - Try to find the CURL client libraries
# Once done this will define
#
# CURL_FOUND - system has libcurl
# CURL_INCLUDE_DIR - the curl include directory
# CURL_LIBRARIES - libcurl (if found) library

FIND_PATH(CURL_INCLUDE_DIR curl/curl.h)
FIND_LIBRARY(CURL_LIBRARIES NAMES curl)

      message(STATUS "Found curl: ${CURL_INCLUDE_DIR} ${CURL_LIBRARIES}")
if(CURL_INCLUDE_DIR AND CURL_LIBRARIES)
   set(CURL_FOUND TRUE)
endif(CURL_INCLUDE_DIR AND CURL_LIBRARIES)

if(CURL_FOUND)
      message(STATUS "Found curl: ${CURL_LIBRARIES}")
else(CURL_FOUND)
      message(STATUS "Not found curl !!! : ${CURL_LIBRARIES}")
endif(CURL_FOUND)

