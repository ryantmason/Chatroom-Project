#include "sender_handler.h"

extern char name[16];
extern char port[6];

/* ******************************************************************** */
/* Creates socket and connects to server                                */
/* ******************************************************************** */
void *handleMessage()
{
    // initialize variables  
    int client_socket;
    struct sockaddr_in server_address;
    struct hostent *server;
    messageDataType message;

    bool joined = false;
    bool shutdown = false;

    char ip[16];
    char serverPort[16];
    char userInput[64];

    int serverIntPort;

    int msgflag;
    char note[64];

    // while not shutdown
    while(!shutdown)
    {
        // get user input
        fgets(userInput, 64, stdin);

        // parse user input
        if(strncmp(userInput, "JOIN", 4) == STR_EQ)
            {
                sscanf(userInput, "%s %s %s", note, ip, serverPort);

                sscanf(serverPort, "%d", &serverIntPort);
            }
        else
        {
            strcpy(note, userInput);
        }

        // check message type
        msgflag = buildMessage( &message, name, note );


        // check if JOIN
        if(msgflag == JOIN )
        {
            if( joined )
            {
                printf("Already connected, please disconnect inorder to join");
            }
    
           // set joined flag
           joined = true;

           // set ip and port in addr struct (may or may not need to do this)
           // create client socket
            client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket < 0)
            {
                // print error 
                perror("Error creating socket");
                exit(EXIT_FAILURE);
            }

            // get the host by name
            server = gethostbyname(ip);
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
            server_address.sin_port = htons(serverIntPort);
          
            printf("Joining chat server...\n");

           // change note to listening port
           strcpy(message.messageNote, port);
        }

        // check if joined
        if(joined)
        {
           // create socket
           client_socket = socket(AF_INET, SOCK_STREAM, 0);

           // connect
           if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
            {
                // print error 
                perror("Error connecting to server");
                exit(EXIT_FAILURE);
            }

           // print the formatted message

           // send the message
           sendMessage(client_socket, &message);

           //close socket
           close(client_socket);

        }
        else
        {
            printf("The client is not connected to a server\n");
        }

        // check if !joined and flag is LEAVE
        if( !joined && msgflag == LEAVE )
        {
            printf("There is no server to leave!\n");
        }
        else if( joined && msgflag == LEAVE)
        {
            joined = false;
        }
        else if( msgflag == SHUTDOWN || msgflag == SHUTDOWN_ALL )
        {
        // shutdown flag
        shutdown = true;
        }
    }
    // print shutdown
    printf("Bye! Shutting Down.\n");

    // pthread exit
    pthread_exit(0);

   return 0;
}
