/*
 *
 *       Filename:  MeetMeMuteUnMute.h
 *
 *    Description:
 *
 *        Created:  07/31/2012 12:08:05 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef MEET_ME_MUTE_UNMUTE_HH
#define MEET_ME_MUTE_UNMUTE_HH

#include <iostream>

#include "Message.h"

class MeetMeMuteUnMute : public Message {
public:
  /**
   * @brief Create a MeetMe mute/unmute request
   * @param conf the conference number
   * @param userNum the user number in the room
   * @param mute true to mute, false to unmute
   **/
  MeetMeMuteUnMute(const std::string& conf, const std::string& userNum, bool mute);
};

#endif
