/*
 *
 *       Filename:  MeetMeListAnswer.h
 *
 *    Description:
 *
 *        Created:  07/31/2012 11:06:05 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 * 2012-01-16 : Guy fix missing ctor
 */

#ifndef MEET_ME_LIST_ANSWER_H__
#define MEET_ME_LIST_ANSWER_H__

#include <vector>

#include "AbstractAnswer.h"
#include "MeetMeUser.h"
#include "Conference.h"

class ConferenceListAnswer : public AbstractAnswer {
public:
  ConferenceListAnswer();

  /**
   * @brief parse the answer
   **/
  void parse();

  /**
   * @brief sets the answer message
   **/
  void setAnswer(const std::string& ans);

  /**
   * @brief returns the number of conferences
   **/
  int getNbConnected();

  /**
   * @brief returns the connected users
   * TODO we should parse this line into a user object
   **/
  std::vector<Conference*>& getConnectedExt();

private:
  std::string              m_raw;
  int                      m_nbConfs;
  int                      m_status;
  std::vector<Conference*> m_connectedExt;

  void parseConference(const std::string& conference);



};
class MeetMeListAnswer : public AbstractAnswer {
public:
  MeetMeListAnswer();

  /**
   * @brief parse the answer
   **/
  void parse();

  /**
   * @brief sets the answer message
   **/
  void setAnswer(const std::string& ans);

  /**
   * @brief returns the number of connected users
   **/
  int getNbConnected();

  /**
   * @brief returns the connected users
   * TODO we should parse this line into a user object
   **/
  std::vector<MeetMeUser*> getConnectedExt();

private:
  std::string m_raw;
  int m_nbUsers;
  int m_status;
  std::vector<MeetMeUser*> m_connectedExt;

  void parseUser(const std::string& user);



};

#endif
