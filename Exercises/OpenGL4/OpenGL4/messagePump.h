#include <SDL.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class MessagePump								// Singleton
{
public:
	static MessagePump _instance;
	static SDL_mutex *mutex_event;
 
	MessagePump() { &getInstance(); mutex_event = SDL_CreateMutex(); }
	~MessagePump() { SDL_DestroyMutex( mutex_event ); } 
	MessagePump(const MessagePump &getInstance());   
	
	MessagePump & operator=(MessagePump &getInstance());
	static MessagePump &getInstance();
	static bool empty();
	static void sendMessage(SDL_Event msg);
	static void sendPriorityMessage(SDL_Event msg);
	static SDL_Event receiveMessage();
	static SDL_Event readMessage();
	static SDL_Event receiveLastMessage();
	static SDL_Event readLastMessage();
	static void deleteMessage();
	static void deleteLastMessage();
	static void lock() { SDL_mutexP( mutex_event ); }
	static void unlock() { SDL_mutexV( mutex_event ); }

	// external declaration
	static list<SDL_Event> messageList;
};