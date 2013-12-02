/*
 *
 *       Filename:  MeetMeList.cpp
 *
 *    Description:
 *
 *        Created:  07/31/2012 10:42:19 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include "MeetMeList.h"
using namespace std;

#include "Actions.h"

ConferenceList::ConferenceList()
  : Message()
{
  append(Actions::Action, Actions::Command);
  append(Actions::Command, "meetme list");
}

MeetMeList::MeetMeList(const string& room):
  Message()
{
  append(Actions::Action, Actions::Command);
  append(Actions::Command, "meetme list " + room);
}

