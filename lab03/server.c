#include "csapp.h"
//get request
void request(int fd) 
{
	struct stat sbuf;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char filename[MAXLINE], cgiargs[MAXLINE];
	rio_t rio;
	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);
	sscanf(buf, "%s %s %s", method, uri, version);
	headerReader(&rio); 
	uri2file(uri, filename, cgiargs); 
	int filesize = sbuf.st_size;

	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer\r\n", buf);
	sprintf(buf, "%sContent-length %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: \r\n", buf);
	Rio_writen(fd, buf, strlen(buf));
	int file_to_send = Open(filename, O_RDONLY, 0);
	char* buf_to_send = malloc(filesize);
	rio_t rio_send_file;
	Rio_readinitb(&rio_send_file, file_to_send);
	Rio_readnb(&rio_send_file, buf_to_send, filesize);
	Close(file_to_send);
	Rio_writen(fd, buf_to_send, filesize);

	fprintf(stdout, "Server Sent %s to Client\n\n", filename);
	free(buf_to_send);
}
//get header
void headerReader(rio_t *rp) 
{
	char buf[MAXLINE];
	Rio_readlineb(rp, buf, MAXLINE);
	 
	while(strcmp(buf, "\r\n")) 
	{
		Rio_readlineb(rp, buf, MAXLINE);
		printf("%s", buf);
	}
    
	return;
}
//get file
void uri2file(char *uri, char *filename, char *cgiargs) 
{
	strcpy(filename, ".");
	strcat(filename, uri);
	if(uri[strlen(uri) - 1] == '/')
	    strcat(filename, "index.html");
}

int main(int argc, char **argv) 
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
	 char clienthostname[MAXLINE], clientport[MAXLINE];
	 if (argc != 2) 
	 {
		 fprintf(stderr, "usage: %s <port>\n", argv[0]);
		 exit(1);
	 }


    port = atoi(argv[1]);
    listenfd = Open_listenfd(port);

	 while (1) 
	 {
		 
		 clientlen = sizeof(clientaddr);
		 connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		 getnameinfo((SA*) &clientaddr, clientlen, clienthostname, MAXLINE,
						 clientport, MAXLINE, 0);
		 fprintf(stdout, "\nConnecting %s:%s\n", 
					clienthostname, clientport);
		 request(connfd);
		 Close(connfd);
    }
}

