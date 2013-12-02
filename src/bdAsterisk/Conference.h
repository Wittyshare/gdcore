#ifndef __Conference_H__
#define __Conference_H__

#include <iostream>

class Conference {
public:
  Conference(const std::string id, const std::string parties, const std::string& marked, const std::string& activity, const std::string& creation, const std::string locked);

  std::string id();
  long        partiesCount();
  std::string marked();
  std::string activity();
  std::string creation();
  std::string locked();

private:
  std::string m_id;
  std::string m_parties;
  std::string m_marked;
  std::string m_activity;
  std::string m_creation;
  std::string m_locked;
};

#endif

