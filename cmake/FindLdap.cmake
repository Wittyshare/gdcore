# - Try to find the LDAP client libraries
# Once done this will define
#
# LDAP_FOUND - system has libldap
# LDAP_INCLUDE_DIR - the ldap include directory
# LDAP_LIBRARIES - libldap + liblber (if found) library
# LBER_LIBRARIES - liblber library

if(UNIX)
   FIND_PATH(LDAP_INCLUDE_DIR ldap.h)
   FIND_LIBRARY(LDAP_LIBRARIES NAMES ldap)
   FIND_LIBRARY(LBER_LIBRARIES NAMES lber)

else(UNIX)
   FIND_PATH(LDAP_INCLUDE_DIR winldap.h)
   FIND_LIBRARY(LDAP_LIBRARIES NAMES wldap32)
endif(UNIX)

if(LDAP_INCLUDE_DIR AND LDAP_LIBRARIES)
   set(LDAP_FOUND TRUE)
   if(LBER_LIBRARIES)
     set(LDAP_LIBRARIES ${LDAP_LIBRARIES} ${LBER_LIBRARIES})
   endif(LBER_LIBRARIES)
#   set(TARGET_LINK_LIBRARIES ${TARGET_LINK_LIBRARIES} ldap lber)
endif(LDAP_INCLUDE_DIR AND LDAP_LIBRARIES)

if(LDAP_FOUND)
      message(STATUS "Found ldap: ${LDAP_LIBRARIES}")
endif(LDAP_FOUND)

