// this file receives the thread 
#include "receiver_handler.h"

extern char name[16];
extern char port[6];

/* ******************************************************************** */
/* Receives message and displays a message accordingly for chat operations */
/* ******************************************************************** */
void *handleServer()
{
    int client_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int clientPort;

    char flagBuffer[4];
    char name[16];
    char note[64];

    messageDataType message;

    int flag;
    sscanf(port, "%d", &clientPort);


    // sent when client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // create unnamed network socket for server to listen on
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(clientPort);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(client_socket, NUM_CONNECTIONS) != 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    
    // server loop
    while (true) 
    {
        // accept connection to client
        int socket = accept(client_socket, NULL, NULL);

        // read message flag
        read_complete(socket, flagBuffer, 4);
        sscanf(flagBuffer, "%d", &flag);

        // read name
        read_complete(socket, name, 16);

        // read note
        read_complete(socket, note, 64);

        buildMessage(&message, name, note);

        // check for shutdown
        if(flag == SHUTDOWN_ALL)
        {
            printf(" %s has shutdown the chat, goodbye\n\n", name);
            exit(0);
        }
        // check for new chat user
        else if(flag == JOIN)
        {
            printf(" %s has joined the chat\n", name);
        }
        // check for if chat user left/shutdown (same thing in terms of server side)
        else if(flag == LEAVE || flag == SHUTDOWN)
        {
            printf(" %s has left the chat\n", name);
        }
        // otherwise, assume note
        else
        {
            printf("%s: %s\n",  name, note);
        }

    }
   return 0;
}


////////////////////////////////////////////////////////////////////
// NOTE: This function was created by Wolf-Dieter Otte on 3/15/23.//
////////////////////////////////////////////////////////////////////
// read from a socket a number of bytes, only returns after all bytes read
int read_complete(int socket, void* buffer, unsigned int size)
{
    ssize_t bytes_read = 0;
    ssize_t bytes_read_all = 0;
    char* buffer_ptr = buffer;
    
    do
    {
        bytes_read = read(socket, buffer_ptr, size);

        switch(bytes_read)
        {
            //case 0:
                // end of file
                //return 0;
            case -1:
                // network error
                return -1;
            default:
                size = size - (int)bytes_read;
                buffer_ptr += bytes_read;
                bytes_read_all += bytes_read;
        }        
    }
    while(size != 0);
    
    return (int) bytes_read_all;
}