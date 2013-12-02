/*
 *
 *       Filename:  ConnectionHandler.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 10:16:43 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include "ConnectionHandler.h"
#include "Actions.h"

using namespace std;

ConnectionHandler::ConnectionHandler(const string& host, const int& port):
  m_host(host),
  m_port(port)
{
}

int ConnectionHandler::connect(const string& username, const string& secret)
{
  m_socket = new TcpSocket(IpAddress(m_host, m_port));
  char buffer[65500] = "\0";
  m_socket->readData(buffer, 65500);
  return authentify(username, secret);
}

int ConnectionHandler::authentify(const string& username, const string& secret)
{
  Message m;
  m.append(Actions::Action, Actions::Login);
  m.append("Username", username);
  m.append("Secret", secret);
  m.append(Actions::Events, Actions::Off);
  send(m);
  sleep(1);
  Answer answer = receive();
  return answer.status();
}

void ConnectionHandler::send(Message& msg)
{
  m_socket->writeData(msg.toString().c_str(), msg.toString().size());
}

void ConnectionHandler::sendAndWaitForAnswer(Message& msg, AbstractAnswer& answer)
{
  send(msg);
  sleep(1);
  char buffer[65500] = "\0";
  int r = m_socket->readData(buffer, 65500);
  string ret(buffer, r);
  answer.setAnswer(ret);
  answer.parse();
}

Answer ConnectionHandler::receive()
{
  char buffer[65500] = "\0";
  int r = m_socket->readData(buffer, 65500);
  string ret(buffer, r);
  return Answer(ret);
}
