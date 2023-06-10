#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

// header files
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "message.h"

#define NUM_CONNECTIONS 5

// data structure for threads to access in order to talk to main and each other
typedef struct clientStatusStruct
    {
        bool shutdown;
        char name[16];
        char port[5];
        char ip  [15];
    } clientStatus;



//function prototypes
void *handleServer();
int read_complete(int socket, void* buffer, unsigned int size);




#endif