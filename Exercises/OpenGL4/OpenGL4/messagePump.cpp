#include "messagePump.h"

// Declare static variables
list<SDL_Event> MessagePump::messageList;
MessagePump MessagePump::_instance;
SDL_mutex * MessagePump::mutex_event;

MessagePump &MessagePump::getInstance()
{
  return _instance;
}

bool MessagePump::empty()
{
	return messageList.empty();
}

void MessagePump::sendMessage(SDL_Event  msg) 
{
	lock();
	messageList.push_back(msg);
	unlock();
}

void MessagePump::sendPriorityMessage(SDL_Event msg) 
{
	lock();
	messageList.push_front(msg);
	unlock();
}

SDL_Event MessagePump::receiveMessage()
{
	lock();
	SDL_Event temp = messageList.front();
	messageList.pop_front();
	unlock();

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
	lock();
	SDL_Event temp = messageList.back();
	messageList.pop_back();
	unlock();

	return temp;
}

void MessagePump::deleteMessage()
{
	lock();
	messageList.pop_front();
	unlock();
}

void MessagePump::deleteLastMessage()
{
	lock();
	messageList.pop_back();
	unlock();
}