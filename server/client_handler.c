// Whenever client connects this file handles client request
#include "client_handler.h"

extern pthread_mutex_t mutex;
extern chatNodeDataType *headNode;

/* ********************************************************************
establishes port ip and connection to client then checks for message type
 and sends message                         
******************************************************************** */
void *handleClient(void *args)
{
    // initialize variables
    // read from the the socket
    // structure for read/write (in order): 
    // message flag (int)
    // name (up to 16 chars, stopped by null char)
    // note (up to 64 chars, stopped by null char)
    int *connectionPtr = (int *)args;
    int connection = *connectionPtr;

    chatNodeDataType *currentNode = headNode;

    int flag;
    char flagBuffer[4];
    char name[16];
    char note[64];
    char ip[16];

    int port;  

    argDataType threadArgs;

    struct sockaddr_in client_address; 
    socklen_t peerAddressLen;

    messageDataType message;

    pthread_t thread;

    // get sender ip
    getpeername(connection, (struct sockaddr *)&client_address, &peerAddressLen);
    getpeername(connection, (struct sockaddr *)&client_address, &peerAddressLen);
    strcpy(ip, inet_ntoa(client_address.sin_addr));

    printf(" Accepted client has an IP of %s\n", ip);

    // read message flag
    read_complete(connection, flagBuffer, 4);
    sscanf(flagBuffer, "%d", &flag);

    // read name
    read_complete(connection, name, 16);

    // read note
    read_complete(connection, note, 64);

    buildMessage(&message, name, note);

    printf("%d, %s sent %s\n", flag, name, note);

    message.chatFlag = flag;

    // check for new chat user
    if(flag == JOIN)
        {
            sscanf(note, "%d", &port);
            addClient(&headNode, ip, port, name);
            
            printf(" %s has joined the chat and is listening on port %d\n\n", name, port);
        }
    // check for if chat user left/shutdown (same thing in terms of server side)
    else if(flag == LEAVE || flag == SHUTDOWN)
        {
            removeClient(&headNode, ip);
            printf(" %s has left the chat\n\n", name);
        }


    // clean up and close connection
    close(connection);

    threadArgs.message = &message;
    
    // send out messages to other client with individual threads
    while( currentNode != NULL )
        {
            printf("%s vs %s\n",currentNode->ip,ip);
            if( strncmp(currentNode->ip,ip, 15) != STR_EQ && strlen(currentNode->ip) > 5 )
                {
                    printf("SEND\n");
                    if (pthread_mutex_lock(&msgMutex) != 0) 
                        {
                         perror("Error locking mutex");
                         exit(EXIT_FAILURE);
                        }
                    threadArgs.node = currentNode;

                    pthread_create(&thread, NULL, distrubuteMessage, (void*)&threadArgs);
                }

            currentNode = currentNode->nextNode;
        }

    if(flag == SHUTDOWN_ALL)
        {
            pthread_join(thread, NULL);
            printf(" %s has shutdown the chat, goodbye\n\n", name);
            clearList(headNode);
            exit(0);
        }

    if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Error unlocking mutex");
            exit(EXIT_FAILURE);
        }

    // exit thread
    pthread_exit(NULL);
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


// individual threads to send messages out to clients
void *distrubuteMessage(void *args)
    {
        // initial variables
        int client_socket;
        struct sockaddr_in server_address;
        struct hostent *server;

        argDataType *threadArgs = (argDataType *)args;

        messageDataType *message = (messageDataType *) threadArgs->message;
        chatNodeDataType *node = (chatNodeDataType *) threadArgs->node;

        // create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        // print error 
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // get the host by name
    server = gethostbyname(node->ip);
    if (server == NULL)
    {
        // print error 
        perror("Error getting host by name");
        exit(EXIT_FAILURE);
    }

    // populate the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    server_address.sin_port = htons(node->port);

    // connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        // print error 
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

        // send the message through socket
        sendMessage(client_socket, message);

        printf(" sent message to %s\n", message->name);

        // close the socket
        close(client_socket);

        if (pthread_mutex_unlock(&msgMutex) != 0) {
            perror("Error unlocking mutex");
            exit(EXIT_FAILURE);
        }
        pthread_exit(0);
    }
