#include "messagePump.h"

// Declare static variables
list<SDL_Event> * MessagePump::messageList;
MessagePump MessagePump::_instance;
SDL_mutex * MessagePump::mutex_event;
unsigned int MessagePump::count=0;

MessagePump & MessagePump::getInstance()
{
	if(MessagePump::count==0) { 
		MessagePump::mutex_event = SDL_CreateMutex();
		//std::cout << "CREATING LIST ";
		messageList = new list<SDL_Event>();
		//std::cout << "ADDRESS: " << &messageList << endl;
	}
	MessagePump::count++;
	return _instance;
}

bool MessagePump::empty()
{
	return messageList->empty();
}

void MessagePump::sendMessage(SDL_Event  msg) 
{
	MutexManager::lockMutex( mutex_event );
	messageList->push_back(msg);
	MutexManager::unlockMutex( mutex_event );
}

void MessagePump::sendPriorityMessage(SDL_Event msg) 
{
	MutexManager::lockMutex( mutex_event );
	messageList->push_front(msg);
	MutexManager::unlockMutex( mutex_event );
}

SDL_Event MessagePump::receiveMessage()
{
	MutexManager::lockMutex( mutex_event );
	SDL_Event temp = messageList->front();
	messageList->pop_front();
	MutexManager::unlockMutex( mutex_event );

	return temp;

}

SDL_Event MessagePump::readMessage()
{
	return messageList->front();
}

SDL_Event MessagePump::readLastMessage()
{
	return messageList->back();
}

SDL_Event MessagePump::receiveLastMessage()
{
	MutexManager::lockMutex( mutex_event );
	
	SDL_Event temp = SDL_Event();
	temp = messageList->back();
	
	messageList->pop_back();
	MutexManager::unlockMutex( mutex_event );

	return temp;
}

void MessagePump::deleteMessage()
{
	MutexManager::lockMutex( mutex_event );
	messageList->pop_front();
	MutexManager::unlockMutex( mutex_event );
}

void MessagePump::deleteLastMessage()
{
	MutexManager::lockMutex( mutex_event );
	messageList->pop_back();
	MutexManager::unlockMutex( mutex_event );
}