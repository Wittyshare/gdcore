/*
 *
 *       Filename:  OriginateCall.cpp
 *
 *    Description:
 *
 *        Created:  07/30/2012 04:05:21 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#include "OriginateCall.h"
#include "Actions.h"

OriginateCall::OriginateCall(
  const std::string& channel, const std::string& context,
  const std::string& exten, const std::string& priority,
  const std::string& callerId, const std::string& timeout,
  const std::string& maxRetries, const std::string& retryTimes ):
  Message()
{
  append(Actions::Action, Actions::OriginateCall);
  append(Actions::Channel, channel );
  append(Actions::Context, context );
  append(Actions::Exten, exten);
  append(Actions::Priority, priority );
  append(Actions::CallerId, callerId );
  append(Actions::Timeout, timeout );
  append(Actions::MaxRetries, maxRetries );
  append(Actions::RetryTime, retryTimes );
}
