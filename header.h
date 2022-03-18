/*-------------------------------------------------------------------------*
 *---									---*
 *---		clientServer.h						---*
 *---									---*
 *---	    This file declares C functions and constants common to both	---*
 *---	the client and server.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Huseyn Mammadov	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Header file inclusion					---//

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>	// For unlink()
#include	<sys/types.h>	// For waitpid(), opendir()
#include	<sys/wait.h>	// For waitpid()
#include	<dirent.h>	// For opendir(), readdir(), closedir()
#include	<sys/socket.h>	// For socket()
#include	<netinet/in.h>	// For sockaddr_in and htons()
#include	<netdb.h>	// For getaddrinfo()
#include	<errno.h>	// For errno var
#include	<sys/stat.h>	// For open(), read(),write(), stat()
#include	<fcntl.h>	// and close()
#include	<signal.h>	// For sigaction()


//---		Definition of constants:				---//

#define		BUFFER_LEN		64

#define		PROGRAM_NAME		"./histogrammer"

#define		FILENAME		"file.txt"
