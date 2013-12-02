/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 10:29:25 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 * =====================================================================================
 */

#include "ConnectionHandler.h"
#include "MeetMeList.h"
#include "MeetMeListAnswer.h"
#include "Conference.h"
#include "MeetMeMuteUnMute.h"
#include "OriginateCall.h"

#include <iostream>
#include <assert.h>
using namespace std;
int main()
{
  cout << "Connecting to AMI interface..." << endl;
  ConnectionHandler manager("192.168.3.201", 5038);
  assert(manager.connect("manapi", "badpasswd") == -1); /* Bad password */
  assert(manager.connect("manapi", "goodpasswd") == 0); /* Good password */
  cout << "Please be sure that minimum 1 conference is created " << endl;
  ConferenceList confList;
  ConferenceListAnswer confAns;
  manager.sendAndWaitForAnswer(confList, confAns);
  cout << "There are " << confAns.getNbConnected() << " active conference" << endl;
  vector<Conference*> vConf = confAns.getConnectedExt();
  for (int i = 0; i < vConf.size(); ++i)
    cout << "Id : " << vConf[i]->id() << " participants : " << vConf[i]->partiesCount() << " marked " << vConf[i]->marked()
         << " activity " <<  vConf[i]->activity() << " creation " <<  vConf[i]->creation() << " locked " <<  vConf[i]->locked() << endl;
  cout << "Please be sure conference 11 is created " << endl;
  MeetMeList list("11");
  MeetMeListAnswer ans;
  manager.sendAndWaitForAnswer(list, ans);
  cout << "There are " << ans.getNbConnected() << " in this conference" << endl;
  vector<MeetMeUser*> vect = ans.getConnectedExt();
  for (int i = 0; i < vect.size(); ++i)
    cout << "User : " << vect[i]->getId() << " " << vect[i]->getName() << " on extension " << vect[i]->getExt() << endl;
  MeetMeMuteUnMute m("11", vect[0]->getId(), false);
  Answer ans2;
  manager.sendAndWaitForAnswer(m, ans2);
  assert(ans2.status() == 0);
  cout << "Mute success" << endl;
  MeetMeMuteUnMute m2("11", vect[0]->getId(), false);
  Answer ans3;
  manager.sendAndWaitForAnswer(m2, ans3);
  assert(ans3.status() == 0);
  cout << "UnMute success" << endl;
  /* Test call */
  std::string channel  = "SIP/40";
  std::string context  = "default";
  std::string extension = "027387924";
  std::string priority = "1";
  std::string callerId = "24";
  // callerId bug : affiche le callerId sur l'appelant aussi plutot que d'afficher le n° appelé
  std::string timeout = "30000";
  std::string maxRetries = "5";
  std::string retryTimes = "300";
  OriginateCall call(channel, context, extension, priority, callerId, timeout, maxRetries, retryTimes);
  Answer ans4;
  manager.sendAndWaitForAnswer(call, ans4);
  assert(ans4.status() == 0);
  return 0;
}
