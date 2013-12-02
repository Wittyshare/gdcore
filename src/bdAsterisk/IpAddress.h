/*
 *
 *       Filename:  IpAddress.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 09:43:26 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef IP_ADDRESS_H__
#define IP_ADDRESS_H__

#include <string>
#include <sstream>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <boost/thread.hpp>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>


class IpAddress {
private:
  int ip;
  std::string host;
  unsigned short port;
  static boost::mutex mut;

public:
  IpAddress();
  IpAddress(unsigned short port);
  IpAddress(int ip, unsigned short port);
  IpAddress(const std::string& host, unsigned short port);
  IpAddress(const IpAddress& ipAddress);
  ~IpAddress();

  void setHost(const std::string& host);
  void setIP(int ip);
  void setPort(unsigned short port);

  std::string getHost() const;
  int getIP() const;
  unsigned short getPort() const;

  operator const bool() {
    return (ip != 0);
  }

  //not a full comparison (args ignored)
  friend inline bool operator !=(const IpAddress& a, const IpAddress& b) {
    return (a.ip != b.ip || a.port != b.port);
  }

  friend inline bool operator ==(const IpAddress& a, const IpAddress& b) {
    return (a.ip == b.ip && a.port == b.port);
  }

  friend std::ostream& operator<<(std::ostream& os, IpAddress& o) {
    os << o.host << ":" << o.port;
    return (os);
  }

  friend std::ostream& operator<<(std::ostream& os, const IpAddress& o) {
    os << o.host << ":" << o.port;
    return (os);
  }

private:
  void resolveAddr();
  void toStringAddr();
  void getLocalIP();
};

#endif


