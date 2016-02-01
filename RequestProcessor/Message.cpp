/////////////////////////////////////////////////////////////////
// Message.cpp - Build and Parse Messages                      //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////

#include "Message.h"

Message::Message(const Command& cmd)
{
  msg_ = cmd + "\n";
  pAttrib = const_cast<char*>(msg_.c_str()) + msg_.size();
}

Message& Message::add(const Attribute& attr)
{
  msg_ += attr + "\n";
  return *this;
}

Message::Command Message::cmd()
{
  size_t pos = msg_.find('\n');
  pAttrib = const_cast<char*>(msg_.c_str()) + pos;
  std::string temp = msg_.substr(0, pos);
  return temp;
}

Message::Attribute Message::next()
{
  if (pAttrib - msg_.c_str() == msg_.size())
    return "";
  else
  {
    size_t start = pAttrib - msg_.c_str() + 1;
    size_t end = msg_.find('\n', pAttrib - msg_.c_str() + 1);
    std::string temp = msg_.substr(start, end - start);
    pAttrib += temp.size() + 1;
    return temp;
  }
}

std::string Message::show()
{
  std::string temp = "\n  message:\n  --------\n  " + cmd();
  Attribute attrib;
  while (true)
  {
    attrib = next();
    if (attrib == "")
      break;
    temp += "\n  " + attrib;
  }
  return temp += "\n";
}

#include <iostream>

#ifdef TEST_MESSAGE

int main()
{
  Message msg("TextSearch : 1");
  std::cout << msg.show();
  msg.add("dstMachine : localhost:8080").add("dstName : TextSearch");
  msg.add("pattern : *.h").add("pattern : *.cpp");
  std::cout << msg.show() << "\n\n";
}

#endif
