#include "messagePump.h"


// Static member initialization. 
MessagePump MessagePump::_instance;

MessagePump &MessagePump::getInstance()
{
  return _instance;
}
