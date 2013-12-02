/*
 *
 *       Filename:  MeetMeUser.h
 *
 *    Description:
 *
 *        Created:  07/31/2012 02:09:15 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef MEET_ME_USER_HH
#define MEET_ME_USER_HH

#include <iostream>

class MeetMeUser {
public:
  MeetMeUser(const std::string id, const std::string ext, const std::string name);

  std::string getId();
  std::string getExt();
  std::string getName();

private:
  std::string m_id;
  std::string m_ext;
  std::string m_name;

};

#endif

