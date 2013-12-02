/*
 *
 *       Filename:  TcpSocket.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 09:28:33 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include "TcpSocket.h"
TcpSocket::TcpSocket(const int fd) :
  socketFD(fd), releaseForced(false)
{
  timeout.tv_usec = NO_TIMEOUT;
  timeout.tv_sec = NO_TIMEOUT;
  this->resolvePeerAddr();
  this->resolveLocalAddr();
}

TcpSocket::TcpSocket(const IpAddress& ipAddress) :
  releaseForced(false), peerAddress(ipAddress)
{
  struct sockaddr_in addr;
  if ((socketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    throw ("Error creating the socket");
  }
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(peerAddress.getPort());
  addr.sin_addr.s_addr = htonl(peerAddress.getIP());
  //Open the socket
  if (::connect(this->socketFD, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
    throw ("Error creating the socket");
  }
  timeout.tv_usec = NO_TIMEOUT;
  timeout.tv_sec = NO_TIMEOUT;
  this->resolveLocalAddr();
}

TcpSocket::~TcpSocket()
{
  boost::mutex::scoped_lock lockRead(this->mutRead);
  boost::mutex::scoped_lock lockWrite(this->mutWrite);
  this->release();
  this->close();
}

int TcpSocket::readData(char* buf, const unsigned int length)
{
  boost::mutex::scoped_lock lock(this->mutRead);
  fd_set fdList;
  FD_ZERO(&fdList);
  FD_SET(socketFD, &fdList);
  int value;
  struct timeval timeout = this->timeout;
  do {
    if (!timeout.tv_usec && !timeout.tv_sec)
      value = ::select(socketFD + 1, &fdList, NULL, NULL, NULL);
    else
      value = ::select(socketFD + 1, &fdList, NULL, NULL, &timeout);
  } while (value == -1 && errno == EINTR);
  if (value == 0) {
    throw ("Error reading from socket - Timeout exception");
  } else if (value == -1) {
    throw ("Error reading from socket");
  }
  if (this->releaseForced)
    return (0);
  int readSize = ::recv(socketFD, buf, length, 0);
  if (readSize == 0) {
    throw ("Error reading from socket - Disconnected");
  } else if (readSize == -1) {
    throw ("Error reading from socket ");
  }
  return (readSize);
}

void TcpSocket::writeData(const char* buf, const unsigned int length)
{
  if (!length) {
    return;
  }
  boost::mutex::scoped_lock lock(this->mutWrite);
  int writeSize = ::send(socketFD, buf, length, 0);
  if (writeSize <= 0) {
    throw ("Error writing to socket");
  }
}

void TcpSocket::setTimeout(const unsigned long timeout)
{
  this->timeout.tv_usec = (timeout % 1000) * 1000;
  this->timeout.tv_sec = timeout / 1000;
}

unsigned long TcpSocket::getTimeout()
{
  unsigned long timeout;
  timeout = this->timeout.tv_sec * 1000;
  timeout += this->timeout.tv_usec / 1000;
  return (timeout);
}

void TcpSocket::release()
{
  this->releaseForced = true;
  ::shutdown(this->socketFD, SHUT_RDWR);
}

void TcpSocket::close()
{
  ::close(this->socketFD);
}

bool TcpSocket::check4readData(const unsigned long timeout)
{
  boost::mutex::scoped_lock lock(this->mutRead);
  struct timeval time;
  fd_set fdList;
  time.tv_usec = (timeout % 1000) * 1000;
  time.tv_sec = timeout / 1000;
  FD_ZERO(&fdList);
  FD_SET(socketFD, &fdList);
  if (select(this->socketFD + 1, &fdList, NULL, NULL, &time) <= 0)
    return (false);
  else
    return (true);
}

IpAddress TcpSocket::getLocalAddress()
{
  return (this->ipAddress);
}

IpAddress TcpSocket::getPeerAddress()
{
  return (this->peerAddress);
}

void TcpSocket::resolvePeerAddr()
{
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(client_addr);
  if (::getpeername(this->socketFD, (struct sockaddr*) &client_addr, &size) == -1) {
    throw ("Error getting socket addr");
  }
  this->peerAddress.setIP(ntohl(client_addr.sin_addr.s_addr));
  this->peerAddress.setPort(ntohs(client_addr.sin_port));
}

int TcpSocket::getSocketFD()
{
  return (this->socketFD);
}

void TcpSocket::resolveLocalAddr()
{
  struct sockaddr_in local_addr;
  socklen_t size = sizeof(local_addr);
  if (::getsockname(this->socketFD, (struct sockaddr*) &local_addr, &size) == -1) {
    throw ("Error getting socket addr");
  }
  this->ipAddress.setPort(ntohs(local_addr.sin_port));
}
