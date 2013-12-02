/*
 *
 *       Filename:  Conference.cpp
 *
 *    Description:
 *
 *        Created:  07/31/2012 02:09:15 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include <boost/lexical_cast.hpp>

#include "Conference.h"
using namespace std;

Conference::Conference(const std::string id, const std::string parties, const std::string& marked, const std::string& activity, const std::string& creation, const std::string locked)
  : m_id(id), m_parties(parties), m_marked(marked), m_activity(activity), m_creation(creation), m_locked(locked)
{
  //  std::cout << "Conference::Conference id = " << m_id << " parties " << m_parties << " marked = " << m_marked << " activity " << m_activity << " creation " << m_creation << " locked " << m_locked << std::endl;
}

string Conference::id()
{
  return m_id;
}

long Conference::partiesCount()
{
  return boost::lexical_cast<long>(m_parties);
}

string Conference::marked()
{
  return m_marked;
}

string Conference::activity()
{
  return m_activity;
}

string Conference::creation()
{
  return m_creation;
}

string Conference::locked()
{
  return m_locked;
}

