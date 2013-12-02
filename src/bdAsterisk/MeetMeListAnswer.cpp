/*
 *
 *       Filename:  MeetMeListAnswer.cpp
 *
 *    Description:
 *
 *        Created:  07/31/2012 11:06:05 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */


using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace boost;

#include "Actions.h"

#include "MeetMeListAnswer.h"

ConferenceListAnswer::ConferenceListAnswer()
  : AbstractAnswer()
{ }

void ConferenceListAnswer::parse()
{
  //List conference result exemple
  //    Action: Command
  //    Command: meetme list
  //
  //    Response: Follows
  //    Privilege: Command
  //    Conf Num       Parties        Marked     Activity  Creation  Locked
  //    12             0001	      N/A        00:00:05  Dynamic   No
  //    11             0001	      N/A        00:00:46  Dynamic   No
  //    * Total number of MeetMe users: 2
  //
  m_nbConfs = 0;
  vector<string> tokens;
  boost::split(tokens, m_raw, boost::is_any_of("\n"));
  string retVal = tokens[0].substr(Reply::Response.size() + 1);
  boost::trim(retVal);
  if (retVal == Reply::Follows)
    m_status =  0;
  else
    m_status = -1;
  /* Check if active conference */
  if (tokens[2] == "No active MeetMe conferences.") {
    m_nbConfs = 0;
  } else {
    for (int i = 3; i < tokens.size() - 3; ++i) {
      //std::cout << "tokens " << i << " " << tokens[i] << endl;
      boost::replace_all(tokens[i], "\t", "");
      parseConference(tokens[i]);
      ++m_nbConfs;
      ++i;
    }
  }
}

void ConferenceListAnswer::setAnswer(const string& ans)
{
  m_raw = ans;
  //    std::cout << m_raw;
}

void ConferenceListAnswer::parseConference(const std::string& conference)
{
  char_separator<char> sep(" ");
  tokenizer< char_separator<char> > tokens(conference, sep);
  int i = 0;
  string id, parties, marked, activity, creation, locked;
  BOOST_FOREACH (const string & t, tokens) {
    if (i == 0)
      id = t;
    if (i == 1)
      parties = t;
    if (i == 2)
      marked = t;
    if (i == 3)
      activity = t;
    if (i == 4)
      creation = t;
    if (i == 5)
      locked = t;
    ++i;
  }
  m_connectedExt.push_back(new Conference(id, parties, marked, activity, creation, locked));
}

int ConferenceListAnswer::getNbConnected()
{
  return m_nbConfs;
}

vector<Conference*>& ConferenceListAnswer::getConnectedExt()
{
  return m_connectedExt;
}



MeetMeListAnswer::MeetMeListAnswer()
  : AbstractAnswer()
{ }

void MeetMeListAnswer::parse()
{
  m_nbUsers = 0;
  vector<string> tokens;
  boost::split(tokens, m_raw, boost::is_any_of("\n"));
  string retVal = tokens[0].substr(Reply::Response.size() + 1);
  boost::trim(retVal);
  if (retVal == Reply::Follows)
    m_status =  0;
  else m_status = -1;
  /* Check if users are connected to this conference */
  if (tokens[2] == "No active MeetMe conferences.") {
    m_nbUsers = 0;
  } else {
    int i = 2;
    while (tokens[i].find_first_of("User") == 0) {
      boost::replace_all(tokens[i], "\t", "");
      parseUser(tokens[i]);
      ++m_nbUsers;
      ++i;
    }
  }
}

void MeetMeListAnswer::setAnswer(const string& ans)
{
  m_raw = ans;
}

void MeetMeListAnswer::parseUser(const std::string& user)
{
  char_separator<char> sep(" ");
  tokenizer< char_separator<char> > tokens(user, sep);
  int i = 0;
  string ext, id, name;
  BOOST_FOREACH (const string & t, tokens) {
    if (i == 2)
      id = t;
    if (i == 3)
      ext = t;
    if (i == 4)
      name = t;
    ++i;
  }
  m_connectedExt.push_back(new MeetMeUser(id, ext, name));
}

int MeetMeListAnswer::getNbConnected()
{
  return m_nbUsers;
}

vector<MeetMeUser*> MeetMeListAnswer::getConnectedExt()
{
  return m_connectedExt;
}
