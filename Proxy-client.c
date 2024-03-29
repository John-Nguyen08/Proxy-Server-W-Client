/* Compilation: gcc -o client client.c
   Execution  : ./client 5000
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[999999];

	if (argc < 2)
	{
		printf("\nPort number is missing...\n");
		exit(0);
	}

//---------------WHILE LOOP GOES HERE-------------------------
while (1)
{


	portno = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error(EXIT_FAILURE, 0, "ERROR opening socket");
	server = gethostbyname("129.120.151.94"); //IP address of server
//	server = gethostbyname("localhost"); //Both in the same machine [IP address 127.0.0.1]

	if (server == NULL)
	{
		printf("\nERROR, no such host...\n");
		exit(0);
	}

	//Connecting with the server
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);
	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		error(EXIT_FAILURE, 0, "ERROR connecting the server...");


	//Sending the message to the server
	printf("\nEnter URL: ");
	bzero(buffer,999999);

	gets(buffer); // read what client wrote and store in buffer

	n = write(sockfd, buffer, strlen(buffer)); // send message to server

	if (n < 0)
	{
		error(EXIT_FAILURE, 0, "ERROR writing to socket");
	}

	//Receiving the message from the client
	bzero(buffer,999999);
	n = read(sockfd, buffer, 999999);


	if ( strcmp(buffer,"bye") == 0 )
        {
		close(sockfd);
                return 0;//Closing the connection
        }


	if (n < 0)
		error(EXIT_FAILURE, 0, "ERROR reading from socket");
	else
	{
		printf("Server has sent: %s\n", buffer);
	}

}
//--------------------END WHILE LOOP--------------------------

	return 0;
}
