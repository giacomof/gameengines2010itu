#include <SDL.h>
#include <string>
#include <list>

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
  static void sendMessage(SDL_Event msg, string recipient);
  static void sendPriorityMessage(SDL_Event msg, string recipient);
  static void sendStringMessage(string msg, string recipient);
  static void sendPriorityStringMessage(string msg, string recipient);
  static SDL_Event receiveMessage();
  static SDL_Event receiveLastMessage();
  static string receiveStringMessage();
  static string receiveLastStringMessage();
  static void deleteMessage();
  static void deleteLastMessage();
  static void deleteStringMessage();
  static void deleteLastStringMessage();

  static list<SDL_Event, string> messageList;
  static list<string, string> stringMessageList;
};
