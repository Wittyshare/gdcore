/*
 *       Filename:  Actions.h
 *
 *    Description:
 *
 *        Created:  07/30/2012 01:36:24 PM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef ACTIONS_H__
#define ACTIONS_H__
#include <iostream>

namespace Actions {
const std::string Action = "Action";
const std::string Login = "Login";
const std::string Events = "Events";
const std::string Off = "off";
const std::string OriginateCall = "Originate";
const std::string Channel = "Channel";
const std::string Context = "Context";
const std::string Exten = "Exten";
const std::string Priority = "Priority";
const std::string CallerId = "CallerID";
const std::string Timeout = "Timeout";
const std::string MaxRetries = "MaxRetries";
const std::string RetryTime = "RetryTime";

const std::string Command = "Command";
const std::string Mute = "MeetmeMute";
const std::string UnMute = "MeetmeUnmute";
}

namespace Reply {
const std::string Response = "Response:";
const std::string Success = "Success";
const std::string Follows = "Follows";
const std::string Message = "Message:";
}

#endif


