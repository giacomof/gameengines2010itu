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
};
