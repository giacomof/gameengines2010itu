#include <SDL.h>
#include <string>
#include <list>
#include <map>

using namespace std;

class MessagePump								// Singleton
{
private: 
  static MessagePump _instance;
 
  MessagePump() {}
  ~MessagePump() {} 
  MessagePump(const MessagePump &);             
  MessagePump & operator=(const MessagePump &);
 
 
public:
  static MessagePump &getInstance();
  static void sendMessage(SDL_Event msg);
  static void sendPriorityMessage(SDL_Event msg);
  static SDL_Event receiveMessage();
  static SDL_Event receiveLastMessage();
  static void deleteMessage();
  static void deleteLastMessage();

  // external declaration
  static list<SDL_Event> messageList;
  //static list<string> recipientList;
};