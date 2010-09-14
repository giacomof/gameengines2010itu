#include "messagePump.h"

// Static member initialization. 
MessagePump MessagePump::_instance;

MessagePump &MessagePump::getInstance()
{
  return _instance;
}

void MessagePump::sendMessage(SDL_Event msg, string recipient) 
{
	messageList.push_back(msg, recipient);
}