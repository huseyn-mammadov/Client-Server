/*-------------------------------------------------------------------------*
 *---									---*
 *---		Node.h							---*
 *---									---*
 *---	    This file declares the Node class and relate functions.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2021 August 16		Huseyn Mammadov	---*
 *---									---*
 *-------------------------------------------------------------------------*/

class	Node
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the address of the left child of '*this', or 'NULL'
  //	if there is no left-child.
  Node*		leftPtr_;

  //  PURPOSE:  To hold the address of the right child of '*this', or 'NULL'
  //	if there is no left-child.
  Node*		rightPtr_;

  //  PURPOSE:  To point to the word being counted.
  char*		wordCPtr_;

  //  PURPOSE:  To tell the count.
  int		count_;


  //  II.  Disallowed auto-generated methods:
 
  Node				();


  Node				(const Node&
				);

  Node&		operator=	(const Node&
				);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to note that word 'wordCPtr' has been
  //	seen once so far.
  Node				(const char*	wordCPtr
				) :
				leftPtr_(NULL),
				rightPtr_(NULL),
				wordCPtr_(strndup(wordCPtr,BUFFER_LEN-1)),
				count_(1)
				{ }
				
  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //	return value.
  ~Node				();

  //  V.  Accessors:
  //  PURPOSE:  To return the address of the left child of '*this', or 'NULL'
  //	if there is no left-child.  No parameters.
  Node*		getLeftPtr	()
				const
				{
				  return(leftPtr_);
				}

  //  PURPOSE:  To return the address of the right child of '*this', or 'NULL'
  //	if there is no left-child.  No parameters.
  Node*		getRightPtr	()
				const
				{
				  return(rightPtr_);
				}

  //  PURPOSE:  To return the address of the word being counted.  No parameters.
  const char*	getWordCPtr	()
				const
				{
				  return(wordCPtr_);
				}

  //  PURPOSE:  To return the count.  No parameters.
  int		getCount	()
				const
				{
				  return(count_);
				}

  //  VI.  Mutators:
  //  PURPOSE:  To note that the address of the left child is now 'newLeftPtr'.
  //	No return value.
  void		setLeftPtr	(Node*	newLeftPtr
				)
				{
				  leftPtr_	= newLeftPtr;
				}

  //  PURPOSE:  To note that the address of the right child is now
  //	'newRightPtr'.  No return value.
  void		setRightPtr	(Node*	newRightPtr
				)
				{
				  rightPtr_	= newRightPtr;
				}

  //  PURPOSE:  To note that the word for '*this' has been seen one more time.
  //	No parameters.  No return value.
  void		incCount	()
				{
				  count_++;
				}

};


//  PURPOSE:  To either increment the count of the node at or under '*nodePtr'
//	that corresponds to word 'wordCPtr' when such a node exists, or to
//	insert a 'new Node()' for 'wordCPtr' under '*nodePtr' that maintains
//	the sorted nature of the subtree.  No return value.
extern
void		insert		(Node*		nodePtr,
				 const char*	wordCPtr
				);


//  PURPOSE:  To print out the subtree pointed to by 'nodePtr' in an in-fix
//	(sorted) fashion.  No return value.
extern
void		print		(const Node*	nodePtr
				);

