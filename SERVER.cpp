/*
    file:   server.c
    Simple sockets server
*/
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
    Variables
*/
#define PORT 4201
#define SERVER "127.0.0.1"

/*
    Main thread
*/
int main (void)
{
    int server_socket, client_socket, client_len;
    struct sockaddr_in server_addr, client_addr;
    char text[80], ch;
    int len, result;

	/* Create unnamed socket and give it a "name" */
    server_socket = socket (PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    result = inet_aton (SERVER, &server_addr.sin_addr);
	/* inet_aton: convert IP addresses from dots-and-number to a binary form and stores it in structure */
    if (result == 0)
    {
        printf ("inet_aton failed\n");
        exit (errno);
    }
    server_addr.sin_port = htons (PORT);

	/* Bind to the socket */
    result = bind (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr));
    if (result != 0)
    {
        perror ("bind");
        exit (errno);
    }

	/* Create a client queue */
    result = listen (server_socket, 1);
    if (result != 0)
    {
        perror ("listen");
        exit (errno);
    }
    printf ("Network server running\n");

	/* Accept a connection. Transaction */
    client_len = sizeof (client_addr);
    client_socket = accept (server_socket, (struct sockaddr *) &client_addr, (socklen_t * __restrict__)&client_len);
    printf ("Connection established to %s\n", inet_ntoa (client_addr.sin_addr));
	/* inet_ntoa: convert IP addresses from dots-and-number to a struct in_addr and back */
    // strcpy (text, "Server> ");
    do
    {
		printf ("from Client : ");
		len = recv (client_socket, &text, sizeof text-1, 0);	/* last byte for zero*/
		ch = text[0];	/* save first char */
		text[len] = 0;		/* last char is zero*/
		puts (text);
		printf ("from Server : ");
		strcpy (text, "OK");
        	send (client_socket, text, strlen (text), 0);
		puts (text);
    }
    while (ch != 'q');
    close (client_socket);
    close (server_socket);
    printf ("Connection terminated.  Server shutting down\n");
    return 0;
}

