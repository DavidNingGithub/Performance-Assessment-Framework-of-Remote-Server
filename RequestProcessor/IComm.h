#ifndef ICOMM_H
#define ICOMM_H
/////////////////////////////////////////////////////////////////
// IComm.h - Communication Interface                           //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////
/*
 *  This interface supports using the RequestProcessor from a 
 *  WPF application.  You may choose to use the GUI with Tester
 *  instead.
 */
#include "Message.h"

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

struct DLL_DECL IComm
{
  virtual ~IComm() {}
  static IComm* Create();
  virtual void PostMessage(const Message& msg) = 0;
  virtual Message GetMessage() = 0;
};

#endif
