/*
 *
 *       Filename:  MessageBuilder.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 11:24:17 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include <iostream>
#include <vector>

#include "Message.h"

using namespace std;

Message::Message()
{
}

Message::Message(const string& msg):
  m_msg(msg)
{
}

void Message::append(const string& key, const string& value)
{
  m_msg += key + ": " + value + "\n";
}

string Message::toString()
{
  return m_msg + "\n";
}


