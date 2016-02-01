#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////
// Message.h - Build and Parse Messages                        //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////

#include <string>

class Message
{
public:
  using Command = std::string;
  using Attribute = std::string;

  Message(const Command& cmd);
  Message() {}
  std::string str();
  Message& add(const Attribute& attrib);
  Command cmd();
  Attribute next();
  void clear();
  std::string show();
private:
  std::string msg_;
  char* pAttrib;
};

inline std::string Message::str() { return msg_; }

inline void Message::clear() { msg_ = ""; }

#endif
