/*
 *
 *       Filename:  Answer.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 01:51:43 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef ANSWER_H__
#define ANSWER_H__

#include <iostream>
#include "AbstractAnswer.h"

class Answer : public AbstractAnswer {

public:
  /**
   * @brief constructor
   **/
  Answer();
  ~Answer();

  /**
   * @brief Create a Answer object from the msg
   * @param msg the message received
   **/
  Answer(const std::string& msg);

  /**
   * @brief return the status from the answer
   * @return -1 if failed, 0 otherwise
   **/
  int status();

  /**
   * @brief set the message received
   **/
  void setAnswer(const std::string& msg);

  /**
   * @brief get the message field
   **/
  const std::string& message();

  /**
   * @brief parse the answer
   **/
  void parse();

private:

  int m_status;

  std::string m_msg;

  std::string m_raw;


};



#endif
