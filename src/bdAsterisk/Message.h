/*
 *
 *       Filename:  Message.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 11:39:23 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef MESSAGE_H__
#define MESSAGE_H__
#include <iostream>
#include <vector>
#include <utility>

class Message {
public:

  /**
   * @brief create a basic message
   **/
  Message();

  /**
   * @brief create a basic message from string
   **/
  Message(const std::string& msg);

  /**
   * @brief return the string form of the message. The trailing '\n' is added at the end.
   **/
  std::string toString();

  /**
   * @brief append a field to the message
   **/
  void append(const std::string& key, const std::string& value);

private:
  std::string m_msg;


};

#endif
