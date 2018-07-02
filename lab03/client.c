#include "csapp.h"

int main(int argc, char **argv) 
{

	char* host = argv[1];
	int port = atoi(argv[2]); 
	char* file = argv[3];
	int clientfd = Open_clientfd(host, port);

	char* send_request = malloc( 
			strlen("GET ") + strlen(file) + strlen(" HTTP/1.1\r\n") +
			strlen("Host: ") + strlen(host) + strlen("\r\n") + 
			strlen("\r\n") + 1);
	strcpy(send_request, "GET ");
  	strcat(send_request, file);
	strcat(send_request, " HTTP/1.1\r\n");
	strcat(send_request, "Host: ");
	strcat(send_request, host);
	strcat(send_request, "\r\n");
	strcat(send_request, "\r\n");	
	Fputs(send_request, stdout);
	Rio_writen(clientfd, send_request, strlen(send_request));
	rio_t rio;
	Rio_readinitb(&rio, clientfd);
	char buf[MAXBUF];
	ssize_t nread;
	while((nread = Rio_readnb(&rio, buf, MAXLINE)) != 0) 
	{
		Fputs(buf, stdout);
	}

	free(send_request);
	Close(clientfd);
}
