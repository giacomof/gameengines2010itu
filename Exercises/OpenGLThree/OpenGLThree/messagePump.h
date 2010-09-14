class MessagePump								// Singleton
{
private: 
  static MessagePump _instance;
 
  MessagePump() {}
  ~MessagePump() {} 
  MessagePump(const MessagePump &);             // intentionally undefined
  MessagePump & operator=(const MessagePump &); // intentionally undefined
 
 
public:
  static MessagePump &getInstance();
};
