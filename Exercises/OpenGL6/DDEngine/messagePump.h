#ifdef __DDEngine
#    define messagePump_D __declspec(dllexport)
#else
#    define messagePump_D __declspec(dllimport)
#endif

#ifndef messagePump__H__
#define messagePump__H__

#include "mutexManager.h"

#include <SDL.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class messagePump_D MessagePump								
{
public:

	// external declaration
	static list<SDL_Event> * messageList;
	static unsigned int count;
	static SDL_mutex * mutex_event;

	// Singleton Definitions
	static MessagePump _instance;
	MessagePump() { &getInstance(); }
	~MessagePump() {};
	MessagePump(const MessagePump & getInstance());   
	MessagePump & operator=(MessagePump & getInstance());
	static MessagePump & getInstance();
	
	static bool empty();
	static void sendMessage(SDL_Event msg);
	static void sendPriorityMessage(SDL_Event msg);
	static SDL_Event receiveMessage();
	static SDL_Event readMessage();
	static SDL_Event receiveLastMessage();
	static SDL_Event readLastMessage();
	static void deleteMessage();
	static void deleteLastMessage();
	

};

#endif