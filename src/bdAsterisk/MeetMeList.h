/*
 *
 *       Filename:  MeetMeList.h
 *
 *    Description:
 *
 *        Created:  07/31/2012 10:42:19 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef MEET_ME_LIST
#define MEET_ME_LIST


#include "Message.h"

#include <iostream>

class ConferenceList : public Message {
public:
  /**
   * @brief create a list of current conferences
   * @param room the room id
   **/
  ConferenceList();

};



class MeetMeList : public Message {
public:
  /**
   * @brief create a MeetMe list participants request
   * @param room the room id
   **/
  MeetMeList(const std::string& room);

};

#endif

