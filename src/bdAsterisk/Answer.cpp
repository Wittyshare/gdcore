/*
 *
 *       Filename:  Answer.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 01:51:43 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */


#include "Answer.h"
#include "Actions.h"

#include <vector>
using namespace std;
#include <boost/algorithm/string.hpp>



Answer::Answer()
{
}

Answer::~Answer()
{
}

Answer::Answer(const string& msg):
  m_raw(msg),
  m_status(-1)
{
  parse();
}

void Answer::parse()
{
  vector<string> tokens;
  std::cout << "m_raw = " << m_raw << std::endl;
  boost::split(tokens, m_raw, boost::is_any_of("\n"));
  for (int iTok = 0; iTok < tokens.size(); ++iTok)
    std::cout << "tokens[" << iTok << "] = " << tokens[iTok] << std::endl;
  string retVal = tokens[0].substr(Reply::Response.size() + 1);
  boost::trim(retVal);
  std::cout << "retVal = " << retVal << std::endl;
  if ( retVal == Reply::Success )
    m_status =  0;
  else
    m_status = -1;
  m_msg = tokens[1].substr(Reply::Message.size() + 1);
  std::cout << "m_msg = " << m_msg << std::endl;
  boost::trim(m_msg);
}

int Answer::status()
{
  return m_status;
}

const string& Answer::message()
{
  return m_msg;
}

void Answer::setAnswer(const string& msg)
{
  m_raw = msg;
}
