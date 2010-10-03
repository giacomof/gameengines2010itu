#include "messagePump.h"

// Declare static variables
list<SDL_Event> MessagePump::messageList;
MessagePump MessagePump::_instance;
SDL_mutex * MessagePump::mutex_event;
unsigned int MessagePump::count=0;

MessagePump MessagePump::getInstance()
{
	if(count==0) MessagePump::mutex_event = SDL_CreateMutex();
	else count++;
	return _instance;
}

bool MessagePump::empty()
{
	return messageList.empty();
}

void MessagePump::sendMessage(SDL_Event  msg) 
{
	AssetManager::lockMutex( mutex_event );
	messageList.push_back(msg);
	AssetManager::unlockMutex( mutex_event );
}

void MessagePump::sendPriorityMessage(SDL_Event msg) 
{
	AssetManager::lockMutex( mutex_event );
	messageList.push_front(msg);
	AssetManager::unlockMutex( mutex_event );
}

SDL_Event MessagePump::receiveMessage()
{
	AssetManager::lockMutex( mutex_event );
	SDL_Event temp = messageList.front();
	messageList.pop_front();
	AssetManager::unlockMutex( mutex_event );

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
	AssetManager::lockMutex( mutex_event );
	SDL_Event temp = messageList.back();
	messageList.pop_back();
	AssetManager::unlockMutex( mutex_event );

	return temp;
}

void MessagePump::deleteMessage()
{
	AssetManager::lockMutex( mutex_event );
	messageList.pop_front();
	AssetManager::unlockMutex( mutex_event );
}

void MessagePump::deleteLastMessage()
{
	AssetManager::lockMutex( mutex_event );
	messageList.pop_back();
	AssetManager::unlockMutex( mutex_event );
}