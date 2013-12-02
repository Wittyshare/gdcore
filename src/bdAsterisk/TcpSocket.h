/*
 *
 *       Filename:  TcpSocket.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 09:28:33 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef TCP_SOCKET_H__
#define TCP_SOCKET_H__


#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <boost/thread/mutex.hpp>

#include "IpAddress.h"

#define NO_TIMEOUT 0

class TcpSocket {
protected:
  int socketFD;
  bool releaseForced;
  IpAddress ipAddress;
  boost::mutex mutRead;
  boost::mutex mutWrite;
  IpAddress peerAddress;
  struct timeval timeout;

public:
  TcpSocket();
  TcpSocket(const int socketFD);
  TcpSocket(const IpAddress& ipAddress);
  virtual ~TcpSocket();

  int readData(char* buf, const unsigned int size);
  void writeData(const char* buf, const unsigned int size);

  void setTimeout(const unsigned long timeout);
  unsigned long getTimeout();

  void release();
  void close();

  bool check4readData(const unsigned long timeout);

  IpAddress getLocalAddress();
  IpAddress getPeerAddress();

  int getSocketFD();

private:
  void resolvePeerAddr();
  void resolveLocalAddr();


};

#endif




