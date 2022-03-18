/*-------------------------------------------------------------------------*
 *---									---*
 *---		wordHistogramServer.c					---*
 *---									---*
 *---	    This file defines a C program that gets file-sys commands	---*
 *---	from client via a socket, executes those commands in their own	---*
 *---	threads, and returns the corresponding output back to the	---*
 *---	client.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2021 August 16		Huseyn Mammadov	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc wordHistogramServer.c callHistogrammer.c -o wordHistogramServer -lpthread -g

//---		Header file inclusion					---//

#include	"header.h"
#include	<pthread.h>	// For pthread_create()


//---		Definition of constants:				---//
const int	ERROR_FD		= -1;


//---		Declarations:						---//

//  PURPOSE:  To make a process that histograms words starting at 'wordIndex'
//  	and count 'wordCount' words, get the word histogram from that process
//	and send it back to the client via socket file-descriptor 'toClientFd'.
//	No return value.
extern
void		callHistogrammer(int		toClientFd,
				 int		wordIndex,
				 int		wordCount
				);


//---		Definition of global vars:				---//

//  PURPOSE:  To be non-zero for as long as this program should run, or '0'
//	otherwise.


//---		Definition of functions:				---//

//  PURPOSE:  To cast 'vPtr' to the pointer type coming from 'doServer()'
//	that points to two integers.  Then, to use those two integers,
//	one as a file descriptor, the other as a thread number, to fulfill
//	requests coming from the client over the file descriptor socket.
//	Returns 'NULL'.
void*		handleClient	(void*		vPtr
				)
{
  //  I.  Application validity check:

  //  II.  Handle client:
  //  II.A.  Get file descriptor:
  int*	   intArray	= (int *) vPtr;
  ///The thread id and the file descriptor are passed, but they come in as a void* pointer.
///Use another pointer to cast back to int*

  int 	   fd		= intArray[0];
  int	   threadNum	= intArray[1];

  free(intArray);
  
  //  RELEASE THE MEMORY
  printf("Thread %d starting.\n",threadNum);

  //  II.B.  Read command:
  char	buffer[2*sizeof(int)];
  int	wordIndex;
  int	wordCount;

  //  GET 2 ints FROM CLIENT
  //  CHANGE THEIR ENDIAN
  read(fd, buffer, sizeof(buffer));
  wordIndex = ntohl(*((int *) buffer));
  wordCount = ntohl(*(((int *) buffer) + 1));
  
  printf("Thread %d received: %d %d\n",threadNum,wordIndex,wordCount);
  callHistogrammer(fd,wordIndex,wordCount);

  //  III.  Finished:
  printf("Thread %d quitting.\n",threadNum);
  //  SOMETHING HERE
  return(NULL);
}


//  PURPOSE:  To run the server by 'accept()'-ing client requests from
//	'listenFd' and doing them.
void		doServer	(int		listenFd
				)
{
  //  I.  Application validity check:

	int ret;
	
	//  II.  Server clients:
	pthread_t		threadId;
	pthread_attr_t	threadAttr;
	int threadCount	= 0;
	
	ret = pthread_attr_init(&threadAttr);
	
	if (ret < 0) {
		printf("pthread_attr_init failed\n");
		return;
	}
	
	ret = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
	
	if (ret < 0) {
		printf("pthread_attr_setdetachstate failed\n");
		return;
		
	}

	while  (1)
	{
		int sockfd, len;
		struct sockaddr_in clientAddr;
		
		len = sizeof(clientAddr);
		/// put the file descriptor from accept() in one of those elements
		sockfd = accept(listenFd, (struct sockaddr *) &clientAddr, &len);
		
		if (sockfd < 0) {
			printf("accept failed\n");
			return;
		}
		else {
			printf("New client connected\n");
		}
		//allocate enough memory for an array of 2 integers

		int* clientFdPtr = (int *) malloc(2 * sizeof(int));
		clientFdPtr[0] = sockfd;
		clientFdPtr[1] = threadCount++;
    ///put the value of threadCount in the other element and increment threadCount.

		///Make a detached thread to handle this new client and have it run handleClient(). Pass the address of your array.

		pthread_create(&threadId, &threadAttr, handleClient, (void *) clientFdPtr);
	}
	
	
	pthread_attr_destroy(&threadAttr);
	
  //  III.  Finished:
}


//  PURPOSE:  To decide a port number, either from the command line arguments
//	'argc' and 'argv[]', or by asking the user.  Returns port number.
int		getPortNum	(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Get listening socket:
  int	portNum;

  if  (argc >= 2)
    portNum	= strtol(argv[1],NULL,0);
  else
  {
    char	buffer[BUFFER_LEN];

    printf("Port number to monopolize? ");
    fgets(buffer,BUFFER_LEN,stdin);
    portNum	= strtol(buffer,NULL,0);
  }

  //  III.  Finished:  
  return(portNum);
}


//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port
				)
{
  //  I.  Application validity check:

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror("socket()");
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0',sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror("bind()");
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


int		main		(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  int 	      port	= getPortNum(argc,argv);
  int	      listenFd	= getServerFileDescriptor(port);
  int	      status	= EXIT_FAILURE;

  if  (listenFd >= 0)
  {
    doServer(listenFd);
    close(listenFd);
    status	= EXIT_SUCCESS;
  }

  //  III.  Finished:
  return(status);
}
