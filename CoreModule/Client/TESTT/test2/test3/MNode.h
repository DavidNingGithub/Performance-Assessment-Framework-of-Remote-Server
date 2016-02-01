#ifndef MNODE_H
#define MNode_H

//////////////////////////////////////////////////////////////////////////
//  MNode.h   -  M-ary node class to use in M-ary tree					//
//  ver 2.0																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Code analyzer for CSE687 Pr1, Sp2014			//
//  Author:		        Shishi Fu									    //
//  Source Author:      Jim Fawcett, CST 4-187, Syracuse University     //
//                      (315) 443-3948, jfawcett@twcny.rr.com           //
//////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * Provides a template-based M-ary Node class that holds a finite number
 * of child nodes.  
 * 
 * Required Files:
 * ===============
 * MNode.h, MNode.cpp, iostream.h, vector.h
 *
 * Build Process:
 * ==============
 * cl /D:TEST_MNODE MNode.cpp
 * 
 * Maintenance History:
 * ====================
 * ver 1.0 : 27 Feb 10
 * - first release
 */


#include <iostream>
#include <vector>

namespace TMTree
{
  /////////////////////////////////////////////////////////////////////////
  // MNode<T> class

  template <typename T>
  class MNode
  {
  public:
    MNode(const T& val);
    MNode(const MNode<T>& node);
    ~MNode();
    MNode<T>* clone();
    MNode<T>& operator=(const MNode<T>& node);
    T& value();
    void addChild(MNode<T>* pNode);
    bool removeChild(MNode<T>* pChild);
    size_t numChildren();
    MNode<T>* nextUnmarkedChild();
	void clearMarks();
    std::string ToString();

	MNode<T>* getPar();
	void setPar(MNode<T>* p);
	int getComplexity();
	void setComplexity(int comp);
	std::vector<MNode<T>*> getChildren();
  private:
    T val_;
	MNode<T>* parent;
    std::vector<MNode<T>*> children;
    size_t visitIndex;
	int complexity;
  };
  //----< return text >----------------------------------------------------

  template <typename T>
  T& MNode<T>::value() { return val_; }
  
  //----< return parent >----------------------------------------------------
  template <typename T>
  MNode<T>* MNode<T>::getPar() { return parent; }

  //----< set parent >----------------------------------------------------
  template <typename T>
  void MNode<T>::setPar(MNode<T>* p) { parent = p; }
  
  //----< promotion constructor >------------------------------------------

  template <typename T>
  MNode<T>::MNode(const T& t) : val_(t), visitIndex(0) { complexity = 1; }

  //----< destructor >-------------------------------------------------------

  template <typename T>
  MNode<T>::~MNode()
  {
    //std::cout << "\n  deleting MNode";
    for(size_t i=0; i<children.size(); ++i)
    {
      delete children[i];
    }
  }
  //----< return copy of self without children >---------------------------

  template <typename T>
  MNode<T>* MNode<T>::clone()
  {
    MNode<T>* pNode = new MNode<T>(value());
    for(size_t i=0; i<children.size(); ++i)
      pNode->addChild(children[i]->clone());
    return pNode;
  }
  //----< copy constructor >-----------------------------------------------

  template <typename T>
  MNode<T>::MNode(const MNode<T>& node) : val_(node.val_)
  {
    for(size_t i=0; i<node.children.size(); ++i)
      children.push_back(node.children[i]->clone());
  }
  //----< assignment operator >----------------------------------------------

  template <typename T>
  MNode<T>& MNode<T>::operator =(const MNode<T>& el)
  {
    if(&el == this) return *this;
    for(size_t i=0; i<children.size(); ++i)
      delete children[i];
    children.clear();
    for(size_t i=0; i<el.children.size(); ++i)
      addChild(el.children[i]->clone());
    return *this;
  }
  //----< add child node >---------------------------------------------------

  template <typename T>
  void MNode<T>::addChild(MNode<T>* pMNode)
  {
    if(pMNode == this)
      throw std::exception("attempting to make node a child of itself!");
	pMNode->setPar(this);
    children.push_back(pMNode);

  }
  //----< return number of children >----------------------------------------

  template <typename T>
  size_t MNode<T>::numChildren() { return children.size(); }

  //----< remove child node - returns false on failure, else true >----------

  template <typename T>
  bool MNode<T>::removeChild(MNode<T>* pMNode)
  {
    std::vector< MNode<T>* >::iterator iter = children.begin();
    for(iter=children.begin(); iter!=children.end(); ++iter)
    {
      if(pMNode == *iter)
      {
        delete pMNode;
        children.erase(iter);
        return true;
      }
    }
    return false;
  }
  //----< return pointer to next unvisited child or null >-----------------

  template <typename T>
  MNode<T>* MNode<T>::nextUnmarkedChild()
  {
    if(children.size() == 0)
      return 0;
    if(visitIndex < children.size())
    {
      return children[visitIndex++];
    }
    return 0;
  }
  //----< "mark" all children as unvisited >-------------------------------

  template <typename T>
  void MNode<T>::clearMarks()
  {
    visitIndex = 0;
  }
  //----< serialize content to string representation of node >-------------

  template <typename T>
  std::string MNode<T>::ToString()
  {
    std::ostringstream temp;
    temp << "<" << value() << ">";
    return temp.str();
  }

  //----< get complexity(node count) of the node >-------------
  template <typename T>
  int MNode<T>::getComplexity()
  {
	  return complexity;
  }

  //----< calculate complexity(node count) of the node >-------------
  template <typename T>
  void MNode<T>::setComplexity(int comp)
  {
	  complexity += comp;
  }

  //----< return children nodes >-------------
  template <typename T>
  std::vector<MNode<T>*> MNode<T>::getChildren()
  {
	  return children;
  }
}
#endif
