#include "messagePump.h"

// actual definition
list<SDL_Event> MessagePump::messageList;

// Static member initialization. 
MessagePump MessagePump::_instance;

MessagePump &MessagePump::getInstance()
{
  return _instance;
}

void MessagePump::sendMessage(SDL_Event msg) 
{
	messageList.push_back(msg);
	//recipientList.push_back(recipient);
}

void MessagePump::sendPriorityMessage(SDL_Event msg) 
{
	messageList.push_front(msg);
	//recipientList.push_front(recipient);
}

SDL_Event MessagePump::receiveMessage()
{
	SDL_Event temp = messageList.front();
	messageList.pop_front();
	//recipientList.pop_front();
	return temp;

}

SDL_Event MessagePump::readMessage()
{
	return messageList.front();
}

SDL_Event MessagePump::readLastMessage()
{
	return messageList.back();
}

SDL_Event MessagePump::receiveLastMessage()
{
	SDL_Event temp = messageList.back();
	messageList.pop_back();
	//recipientList.pop_back();
	return temp;
}

void MessagePump::deleteMessage()
{
	messageList.pop_front();
	//recipientList.pop_front();
}

void MessagePump::deleteLastMessage()
{
	messageList.pop_back();
	//recipientList.pop_back();
}