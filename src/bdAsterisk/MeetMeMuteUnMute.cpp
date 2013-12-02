/*
 *
 *       Filename:  MeetMeMuteUnMute.cpp
 *
 *    Description:
 *
 *        Created:  07/31/2012 12:08:05 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */
#include "MeetMeMuteUnMute.h"
#include "Actions.h"

using namespace std;

MeetMeMuteUnMute::MeetMeMuteUnMute(const string& conf, const string& userNum, bool mute): Message()
{
  if (mute)
    append(Actions::Action, Actions::Mute);
  else
    append(Actions::Action, Actions::UnMute);
  append("MeetMe", conf);
  append("Usernum", userNum);
}
