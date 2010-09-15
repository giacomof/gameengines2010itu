#include <SDL.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class MessagePump								// Singleton
{
public:
	
	static MessagePump _instance;
 
	MessagePump() { getInstance(); }
	~MessagePump() { delete this; } 
	
	MessagePump & operator=(const MessagePump &);
	static MessagePump &getInstance();
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
	//static list<string> recipientList;

	MessagePump(const MessagePump &);   
};