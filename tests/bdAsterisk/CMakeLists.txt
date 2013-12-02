cmake_minimum_required(VERSION 2.8)

PROJECT(Asterisk_test)

SET(APPNAME Asterisk_test)


SET(APPNAME_SRCS
    ../../src/bdAsterisk/TcpSocket.cpp
    ../../src/bdAsterisk/IpAddress.cpp
    ../../src/bdAsterisk/ConnectionHandler.cpp
    ../../src/bdAsterisk/Message.cpp
    ../../src/bdAsterisk/Answer.cpp
    ../../src/bdAsterisk/AbstractAnswer.cpp
    ../../src/bdAsterisk/OriginateCall.cpp
    ../../src/bdAsterisk/MeetMeList.cpp
    ../../src/bdAsterisk/MeetMeListAnswer.cpp
    ../../src/bdAsterisk/MeetMeMuteUnMute.cpp
    ../../src/bdAsterisk/MeetMeUser.cpp
    ../../src/bdAsterisk/Conference.cpp
    test.cpp
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
       ../../src/bdAsterisk
    )


#FIND_PACKAGE(Boost COMPONENTS threads required)

SET(BOOST_LIB /usr/lib/libboost_thread.so)
SET(BOOST_LIB_s /usr/lib/libboost_system.so)

SET ( APP_LIBRARIES
    ${BOOST_LIB}
    ${BOOST_LIB_s}
)

ADD_EXECUTABLE(
    ${APPNAME}
    ${APPNAME_SRCS} 
    )

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )
