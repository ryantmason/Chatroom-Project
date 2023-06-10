// server main
#include "main.h"

// Nicholas Henderson, Ryan Mason

/* ******************************************************************** */
/*  server loop, listens for clients connection and relays it           */
/* ******************************************************************** */
int main(int argc, char** argv) {
    int server_socket;                 // descriptor of server socket
    
    char* properties_file = "test.properties";
    Properties* properties;
    char* key = "MY_PORT";
    char* portBuffer;
    int port;
    
    properties = property_read_properties(properties_file);
    portBuffer = property_get_property(properties, key);

    sscanf(portBuffer, "%d", &port);

    printf("============\n");
    printf("SERVER START\n");
     printf("============\n\n");
    
    struct sockaddr_in server_address; // for naming the server's listening socket

    // sent when client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(port);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) 
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) != 0) 
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) 
    {
        perror("Error initializing mutex");
        exit(EXIT_FAILURE);
    }

    printf(" server listening on port %d\n\n", port);
    
    // server loop
    while (TRUE) {
        
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

	if (pthread_mutex_lock(&mutex) != 0) 
        {
            perror("Error locking mutex");
            exit(EXIT_FAILURE);
        }

        // create thread to handle the client's request
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleClient, (void*)&client_socket) != 0) 
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
        
        // detach the thread so that we don't have to wait (join) with it to reclaim memory.
        // memory will be reclaimed when the thread finishes.
        if (pthread_detach(thread) != 0) 
        {
            perror("Error detaching thread");
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_mutex_destroy(&mutex) != 0) 
    {
        perror("Error destroying mutex");
        exit(EXIT_FAILURE);
    }
    
}
