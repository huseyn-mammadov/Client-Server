/*-------------------------------------------------------------------------*
 *---									---*
 *---		Node.cpp						---*
 *---									---*
 *---	    This file defines the methods and functions related to	---*
 *---	class Node.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2021 August 16		Huseyn Mammadov	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"header.h"
#include	"Node.h"

//  PURPOSE:  To release the resources of '*this'.  No parameters.  No
//	return value.
Node::~Node			()
{

	free(wordCPtr_);
	
	if (leftPtr_ != NULL)
		delete leftPtr_;
	
	if (rightPtr_ != NULL)
		delete rightPtr_;
}


//  PURPOSE:  To either increment the count of the node at or under '*nodePtr'
//	that corresponds to word 'wordCPtr' when such a node exists, or to
//	insert a 'new Node()' for 'wordCPtr' under '*nodePtr' that maintains
//	the sorted nature of the subtree.  No return value.
void		insert		(Node*		nodePtr,
				 const char*	wordCPtr
				)
{
  int	compRes	= strcmp(nodePtr->getWordCPtr(),wordCPtr);

  if  (compRes > 0)
  {
   
	if (nodePtr->getLeftPtr() != NULL) {
		insert(nodePtr->getLeftPtr(), wordCPtr);
	}
	else {
		Node* newNode = new Node(wordCPtr);
		nodePtr->setLeftPtr(newNode);
	}
  }
  else
  if  (compRes < 0)
  {
    
	if (nodePtr->getRightPtr() != NULL) {
		insert(nodePtr->getRightPtr(), wordCPtr);
	}
	else {
		Node* newNode = new Node(wordCPtr);
		nodePtr->setRightPtr(newNode);
	}
  }
  else
  {

	nodePtr->incCount();
  }
}


//  PURPOSE:  To print out the subtree pointed to by 'nodePtr' in an in-fix
//	(sorted) fashion.  No return value.
void		print		(const Node*	nodePtr
				)
{

  if (nodePtr == NULL)
	  return;
  
  print(nodePtr->getLeftPtr());
  printf("%d\t%s\n",nodePtr->getCount(),nodePtr->getWordCPtr());
  print(nodePtr->getRightPtr());
}
