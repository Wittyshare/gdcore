/*
 *
 *       Filename:  MeetMeUser.cpp
 *
 *    Description:
 *
 *        Created:  07/31/2012 02:09:15 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

using namespace std;

#include "MeetMeUser.h"

MeetMeUser::MeetMeUser(const string id, const string ext, const string name):
  m_name(name),
  m_ext(ext),
  m_id(id)
{
}

string MeetMeUser::getName()
{
  return m_name;
}

string MeetMeUser::getExt()
{
  return m_ext;
}

string MeetMeUser::getId()
{
  return m_id;
}
