/*
 *
 *       Filename:  ConnectionHandler.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 10:14:06 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef CONNECTION_HANDLER_H__
#define CONNECTION_HANDLER_H__

#include <iostream>

#include "TcpSocket.h"
#include "Message.h"
#include "Answer.h"
#include "Actions.h"

class ConnectionHandler {

public:
  /**
   * @brief constructor
   * @param host the host to connect to
   * @param port the port to connect to
   **/
  ConnectionHandler(const std::string& host, const int& port);

  /**
   * @brief Connect to the asterisk AMI interface and authentifies
   * @param username the login username
   * @param secret the login secret.
   **/
  int connect(const std::string& username, const std::string& secret);

  /**
   * @brief sends a basic message. receive should be used to read the answer
   **/
  void send(Message& msg);

  /**
   * @brief sends the message and waits for the answer. The message type must coincide with the Answer type
   * @example : If sending a MeetMeList message, answer should be a MeetMeListAnswer object
   **/
  void sendAndWaitForAnswer(Message& msg, AbstractAnswer& answer);

  /**
   * @brief receive a basic message
   **/
  Answer receive();

protected:
  std::string m_host;
  int m_port;
  TcpSocket* m_socket;

  int authentify(const std::string& username, const std::string& secret);

};

#endif

