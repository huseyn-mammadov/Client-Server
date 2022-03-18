/*-------------------------------------------------------------------------*
 *---									---*
 *---		callHistogrammer.c					---*
 *---									---*
 *---	    This file defines a histogram-calling function.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2021 August 16		Huseyn Mammadov	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc wordHistogramServer.c callHistogrammer.c -o wordHistogramServer -lpthread

//---		Header file inclusion					---//

#include	"header.h"


//  PURPOSE:  To make a process that histograms words starting at 'wordIndex'
//  	and count 'wordCount' words, get the word histogram from that process
//	and send it back to the client via socket file-descriptor 'toClientFd'.
//	No return value.
void		callHistogrammer(int		toClientFd,
				 int		wordIndex,
				 int		wordCount
				)
{
  int	childToParent[2];
  pid_t	childPid;
  int	endOrErr	= 0;	/
	
	//Make the pipe.
  // AND THEN CHILD
	if (pipe(childToParent) == -1)
		exit(-1);
	///Make a child process.

	childPid = fork();

  if  (childPid == 0)
  {
    char	wordIndexBuffer[BUFFER_LEN];
    char	wordCountBuffer[BUFFER_LEN];

	char *hist_args[] = {"./histogrammer", wordIndexBuffer, NULL};
	///close() unnecessary pipe file descriptor

    //  CLOSE AND RE-DIRECT
	close(childToParent[0]);	
	dup2(childToParent[1], 1);

	snprintf(wordIndexBuffer, sizeof(wordIndexBuffer), "%d", wordIndex);
		
	//  CALL PROGRAM_NAME WITH COMMAND LINE ARGUMENTS
	execvp("./histogrammer", hist_args);
	endOrErr = htonl(-1);
	*((int *) wordCountBuffer) = endOrErr;
	wordCountBuffer[4] = '\n';
	write(childToParent[1], wordCountBuffer, 5);
	
    //  HANDLE ERROR CASE
    exit(EXIT_FAILURE);
  }

  //  CLOSE, HANG-OUT, SEND-SIGNAL
  close(childToParent[1]);
  //dup2(childToParent[0], toClientFd);
  
  sleep(wordCount);
  
  kill(childPid, SIGINT);
  
  char	buffer[BUFFER_LEN];
  FILE*	inputPtr	= fdopen(childToParent[0],"r");

  while  (fgets(buffer,BUFFER_LEN,inputPtr) != NULL)
  {
    int		count;
    char	word[BUFFER_LEN];

    word[0]	= '\0';

    //  PARSE, CONVERT AND SEND TO CLIENT
    sscanf( buffer, "%d %s", &count, word);
	count = htonl(count);
	write(toClientFd, &count, 4);
	write(toClientFd, word, strlen(word));
	write(toClientFd, "\n", 1);
  }

  endOrErr = htonl(0);
  write(toClientFd, &endOrErr, 4);
  write(toClientFd, "\n", 1);
	
 
  close(childToParent[1]);
  wait(NULL);
}
