#pragma once

#ifndef messagePump__H__
#define messagePump__H__

#include "assetManager.h"

#include <SDL.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class MessagePump								// Singleton
{
public:
	static MessagePump _instance;
	static SDL_mutex * mutex_event;
 
	MessagePump() { getInstance(); }
	~MessagePump() {} 
	MessagePump(const MessagePump &getInstance());   
	
	MessagePump & operator=(MessagePump getInstance());
	static MessagePump getInstance();
	
	static bool empty();
	static void sendMessage(SDL_Event msg);
	static void sendPriorityMessage(SDL_Event msg);
	static SDL_Event receiveMessage();
	static SDL_Event readMessage();
	static SDL_Event receiveLastMessage();
	static SDL_Event readLastMessage();
	static void deleteMessage();
	static void deleteLastMessage();
	
	// external declaration
	static list<SDL_Event> messageList;
	static unsigned int count;
};

#endif