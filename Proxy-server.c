/* Compilation: gcc -o server server.c
   Execution  : ./server 5000
   Function: This Server Code is a Proxy server. It will take a Clients URL and see within it own cache if the website is saved. If it is it will make sure the website is up to date and if it is, send the HTML objects back to the client. If the URL does not exist it or is not updated it will obtain the objects via "get" and send it to the client and save the object and URL within it own storage.  
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <error.h>
#include <string.h>



int main(int argc, char *argv[])
{
//---------------------------------------------------------------

	int sockfd, newsockfd, portno, clilen, n;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[256]; // buffer that carries the message between client and server

	FILE *fp; // This is the list of websites from the client
	FILE *fp2; // this stream will be use to read URL.html body to 'streambuff'
	fp = fopen("list.txt","w+"); // create a text doc named list.txt that deletes everything when i execute
	fclose(fp); // close stream cuz i just wanted to create the text file

	char streambuff[999999]; // this is buffer will carry between read and write.
	char cachebuff[100];//this buffer is used for checking URL in list.txt
	int d = 0; // will be a bool that tells me if i need to ping orgin server or not

	char getting[100];//NOTE: i already added the space
	char head[100]; //read above notes.
	strcpy(getting, "GET / HTTP/1.1Host: ");
	strcpy(head, "HEAD / HTTP/1.1Host: ");
//---------------------------------------------------------------

	if(argc < 2) // CMD line paramater error checking
	{
		printf("\nPort number is missing...\n");
		exit(0);
	}
//---------------------------------------------------------------

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating the server

//---------------------------------------------------------------

	if (sockfd < 0)
		error(EXIT_FAILURE, 0, "ERROR opening socket");

//---------------------------------------------------------------

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

//---------------------------------------------------------------

	if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		error(EXIT_FAILURE, 0, "ERROR binding");

//---------------------------------------------------------------


	while(1)
	{
//----------Start of the code when start up is correct-----------
	printf("\nServer Started and listening to the port %d\n", portno);
    	listen(sockfd, 5);


//----------------------listening--------------------------------
		//Connecting with the client
		clilen=sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)	//Error Checking
		{
				error(EXIT_FAILURE, 0, "ERROR on accept");
		}


		printf("\nClient is connected...\n"); //Client is connected, start taking.

		//Receiving the message from the client
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);


		if (strcmp (buffer,"bye") == 0 )// check if I should close connection
		{
                        bzero(buffer,256);
			strcpy(buffer, "bye");
                        n = write(newsockfd, buffer, strlen(buffer));
                        close(newsockfd);       //Closing the connection
                }



	   	if(n < 0)
			error(EXIT_FAILURE, 0, "ERROR reading from socket");
	   	else
	   	{


			printf("\nClient requesting webpage is: %s ...searching cache\n", buffer);

			fp = fopen("list.txt","r"); //open FP refNUM 1
			while (fgets (cachebuff,255,(FILE*)fp) != NULL)//get the every line in cache EX google.comp; yahoo.com; 
			{
				if (strcmp (cachebuff,buffer) == 0) // compare client with the list && if hit go through.
				{
					printf("\n Webpage is in our cache, sending to client... \n");
					strcat(buffer,".html");//apppend 'buffer' with .html and set it to 'appendstring' EX www.google.com.html
					fp2 = fopen(buffer,"r");//open the html body of said URL
					while (fgets (streambuff,999999,(FILE*)fp2) != NULL) // read every line in HTML body
					{
						n = write(newsockfd, streambuff, strlen(streambuff)); // send it to  client
					}
					printf("Sent complete webpage\n");
					fclose(fp2); //close the pserver to client
					fclose(fp); // close the read "check cache list" stream
					d = 1;//set d to 1 tell me that i cache list was a hit, i dont need to ping orgin
					bzero(buffer,256);
				}
				else
					d = 0;
			}
			fclose(fp); //close FP RefNUM 1; if the "fpclose" inside the "if"  does not exec
			printf("\nSite is not in cache...Pinging server\n");







			if ( d == 0) // tells me client URL is NOT in cache list, ping orgin.
			{
				int verifyline = 1;
				strcat(head,buffer);// head is now GET w/ client URL
				fp = popen(head,"r"); //execute HEAD function. RefNUM 2

				printf("\nbuffer is: %s \n",buffer);

				bzero(streambuff,999999);
				while (fgets(streambuff,999999,(FILE*)fp) != NULL)
				{
					if ( verifyline == 12 && ( strcmp(streambuff,"200 OK\n") == 0 ) )
					{
						fclose(fp); // found the verify 200 response, now close. RefNUM 2

						printf("\n HTTP response is 200 OK \n");
						fp = fopen("list.txt","a");
						fputs(buffer,fp);
						fclose(fp);

						char tempbuff1[100]; //temp buff
						strcpy(tempbuff1,buffer); //add URL to temp
						strcat(tempbuff1,".html"); //add .html to temp buff
						strcat(getting,buffer); // the final get command with client URL
						fp = popen(getting,"r"); // open pipe stream and execute command
						fp2 = fopen(tempbuff1,"w"); //open URL.html and stream write.
						bzero(streambuff,999999);

						//below is code that puts html body content intoa new text file
						while (fgets(streambuff,999999,(FILE*)fp) != NULL )
						{
							fputs(streambuff,fp2);
						}
						fclose(fp);
						fclose(fp2);

						//send URL html body to client.
						fp2 = fopen(tempbuff1,"r");
						bzero(streambuff,999999);
						while (fgets (streambuff,999999,(FILE*)fp2) != NULL) // read every line in H$
                                        	{
                                        	        n = write(newsockfd, streambuff, strlen(streambuff)); // send it to $
                                        	}
						fclose(fp2);
						break;
					}

/*					if (verifyline == 12 && ( strcmp(streambuff,"200 OK\n") != 0) )
					{
						fclose(fp);// found the verify 200 response, now close. RefNUM 2

						fp = fopen("tempfile.txt","w");
						printf("\n IM NOT in 200 OK \n");
						strcat(head,buffer);
						fp2 = popen(head,"r");
						bzero(streambuff,999999);
						while (fgets(streambuff,999999,(FILE*)fp2) != NULL)
						{
							fputs(streambuff,fp);
						}
						fclose(fp2);
						fclose(fp);
						fp2 = fopen("tempfile.txt","r");
						bzero(streambuff,999999);
						while (fgets (streambuff,999999,(FILE*)fp2) != NULL) // read every l$
                                                {
                                                        n = write(newsockfd, streambuff, strlen(streambuff)); // sen$
                                                }
						fclose(fp2);
						system("rm tempfile.txt");
					}*/

					verifyline++;

				}
			}
			else// client URL was in cache and sent
			{
				d = 0;
			}

		printf("------------------------------------------------------------------");
// The below is reference.
//			bzero(buffer,256);
//			strcpy(buffer, "Total Vowel in your message is: ");
//		   	n = write(newsockfd, buffer, strlen(buffer));


		}
	}
return 0;
}
