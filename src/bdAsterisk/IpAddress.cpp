/*
 *
 *       Filename:  IpAddress.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 09:43:26 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include "IpAddress.h"

#define inaddrr(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])
#define IFRSIZE ((int)(size * sizeof (struct ifreq)))

const std::string localhost = "127.0.0.1";


boost::mutex IpAddress::mut;

IpAddress::IpAddress()
{
  this->host = localhost;
  this->ip = 0;
  this->port = 0;
  this->getLocalIP();
  this->resolveAddr();
}

IpAddress::IpAddress(unsigned short port)
{
  this->host = localhost;
  this->ip = 0;
  this->port = port;
  this->getLocalIP();
  this->resolveAddr();
}

IpAddress::IpAddress(int ip, unsigned short port)
{
  this->ip = ip;
  this->port = port;
  this->toStringAddr();
}

IpAddress::IpAddress(const std::string& host, unsigned short port)
{
  this->host = host;
  this->port = port;
  this->resolveAddr();
}

IpAddress::IpAddress(const IpAddress& ipAddress)
{
  this->host = ipAddress.host;
  this->ip = ipAddress.ip;
  this->port = ipAddress.port;
}

IpAddress::~IpAddress()
{
}

void IpAddress::setHost(const std::string& host)
{
  this->host = host;
  this->resolveAddr();
}

void IpAddress::setIP(int ip)
{
  this->ip = ip;
  this->toStringAddr();
}

void IpAddress::setPort(unsigned short port)
{
  this->port = port;
}

std::string IpAddress::getHost() const
{
  return (this->host);
}

int IpAddress::getIP() const
{
  return (this->ip);
}

unsigned short IpAddress::getPort() const
{
  return (this->port);
}

void IpAddress::resolveAddr()
{
  boost::mutex::scoped_lock lock(mut);
  in_addr sin_addr;
  char* host = (char*) this->host.c_str();
  struct hostent* h;
  h = gethostbyname(host);
  if (!h) {
    std::cerr << "IpAddress error was " << host << " : " << strerror(errno) << std::endl;
    ip = ntohl(0x7F000001L);
  } else {
    sin_addr = *(struct in_addr*) h->h_addr;
    ip = ntohl(sin_addr.s_addr);
  }
}

void IpAddress::toStringAddr()
{
  std::ostringstream strm;
  strm << ((int) (ip >> 24) & 0xFF) << ".";
  strm << ((int) (ip >> 16) & 0xFF) << ".";
  strm << ((int) (ip >> 8) & 0xFF) << ".";
  strm << ((int) (ip >> 0) & 0xFF);
  this->host = strm.str();
}

void IpAddress::getLocalIP()
{
  std::string host;
  int sockfd, size = 1;
  struct ifreq* ifr = NULL;
  struct ifconf ifc;
  struct sockaddr_in sa;
  if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP))) {
    std::cerr << "IpAddress error was " << this->host << " : " << strerror(errno) << std::endl;
    return;
  }
  ifc.ifc_len = IFRSIZE;
  ifc.ifc_req = NULL;
  do {
    ++size;
    /* realloc buffer size until no overflow occurs */
    if (NULL == (ifc.ifc_req = (ifreq*) realloc(ifc.ifc_req, IFRSIZE))) {
      std::cerr << "IpAddress error was Out of memory." << std::endl;
      return;
    }
    ifc.ifc_len = IFRSIZE;
    if (ioctl(sockfd, SIOCGIFCONF, &ifc)) {
      std::cerr << "IpAddress error was ioctl SIOCFIFCONF." << std::endl;
      return;
    }
  } while (IFRSIZE <= ifc.ifc_len);
  ifr = ifc.ifc_req;
  for (; (char*) ifr < (char*) ifc.ifc_req + ifc.ifc_len; ++ifr) {
    if (ifr->ifr_addr.sa_data == (ifr + 1)->ifr_addr.sa_data) {
      continue; /* duplicate, skip it */
    }
    if (ioctl(sockfd, SIOCGIFFLAGS, ifr)) {
      continue; /* failed to get flags, skip it */
    }
    host = inet_ntoa(inaddrr(ifr_addr.sa_data));
    if (host != "127.0.0.1")
      this->host = host;
  }
  if (ifc.ifc_req)
    free(ifc.ifc_req);
  close(sockfd);
}


