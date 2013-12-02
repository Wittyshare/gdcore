/*
 *
 *       Filename:  OriginateCall.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 04:05:21 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef ORIGINATE_CALL_H__
#define ORIGINATE_CALL_H__
#include <iostream>
#include "Message.h"
class OriginateCall : public Message {
public:
  OriginateCall(const std::string& channel, const std::string& context,
                const std::string& exten, const std::string& priority,
                const std::string& callerId, const std::string& timeout,
                const std::string& maxRetries, const std::string& retryTimes );

  OriginateCall();
};

#endif
