/*
 * Programmer: Matas Noreika Wed Jul 16 03:00:23 PM IST 2025
 * Purpose: Example program that showcases how to create a simple TCP client that will connect to a server and print back out the response message
*/

#include <sys/socket.h>//POSIX c main header for berkley sockets
#include <sys/types.h>//POSIX c header for various type definitions
#include <arpa/inet.h>//POSIX c header for functions to manipulate IP addresses
#include <netinet/in.h>//POSIX c header for internet protocol definition and address families (IPv4/IPv6) 
#include <unistd.h>//POSIX c header for general utilites like file descriptor manipulation
#include <stdio.h>//libc header for input/output
#include <stdlib.h>//libc header for general utilities like memeory dynamic allocation malloc, calloc
#include <string.h>//libc header for string manipulation

int main(char** argv, int argc){//start of main method
	
	//variable to store the server address to connect to
	struct sockaddr_in serverAddr;	
	//variable to store the socket file descriptor
	int sfd;
	
	//request string to send once connected to google to get response
	char* requestStr = "GET / HTTP/1.1\r\nHost: www.google.com\r\nAccept-Language: en\r\n\r\n";

	//call to create a socket using the TCP transport protocol
	//PF_INET - tells that the domain of the socket is related to the internet 
	//SOCK_STREAM - tells that the socket will behave as a presistant connection (pipe)
	//IPPROTO_TCP - tells that the socket will use the TCP protocol to communicate over the pipe 
	sfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//error handling for socket creation
	if(sfd == -1){
		//print a red message out to error stream
		fprintf(stderr,"\e[31mCannot create socket!\e[m\n");
		//return the standard error code from stdlib
		exit(EXIT_FAILURE);
	}

	//set the structure memeory to all 0's
	memset(&serverAddr,0,sizeof(serverAddr));
	
	//sets the address type to internet domain
	serverAddr.sin_family = AF_INET;
	//sets the port as 80 in network byte order
	serverAddr.sin_port= htons(80);
	//converts the IPv4 string address to network byte order format
	//we are connecting to google (use can obtain the IPv4 using ping google.com)
	inet_pton(AF_INET,"209.85.202.139",&serverAddr.sin_addr);
	
	//check if connection failed
	//you need to typecast the sockaddr_in to type of sockaddr
	if(connect(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1){
		//print an error message and exit with standard error code
		fprintf(stderr,"\e[31mCannot connect to google!\e[m\n");
		exit(EXIT_FAILURE);
	}
	
	//send the request
	int total = strlen(requestStr), sent = 0, bytes;
	
	do{//start of send request loop
		
		//attempt to write request string to server (returns the number of bytes sent)
		bytes = write(sfd, requestStr+sent,total-sent);
		//check if the number of bytes sent is non valid (error)	
		if(bytes < 0){
			fprintf(stderr,"\e[31mError writing to server!\e[m\n");
		}
		//check if we are finished sending request
		if(bytes == 0){
			break;
		}
		//append written byte value
		sent += bytes;
	
	}while( sent < total);//end of send request loop

	//recieve the response
	//reponse buffer of 4096 bytes (better to dynamically allocated based on the content-size given in header of response)
	char response[4096];
	total = sizeof(response) - 1;//remember the null terminator at the end of string counts to the total size
	int recieved = 0;

	do{//start of response read loop
		
		//attempt to read into the response buffer from socket
		bytes = read(sfd,response+recieved,total-recieved);
		
		//check if error occurred while reading from socket
		if(bytes < 0){
			fprintf(stderr, "\e[31mError occured while reading response!\e[m\n");
		}
		
		//response is fully read
		if(bytes == 0){
			break;
		}
		//append the value of the total response read
		recieved += bytes;

	}while(recieved < total);//end of response read loop

	//print back the contents of the response
	printf("Response:\r\n%s\n",response);

	//always close a socket as you would a file
	close(sfd);
	//return success standard error code
	return EXIT_SUCCESS;
}//end of main method
