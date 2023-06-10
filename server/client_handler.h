// protect from multiple compiling
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <netdb.h>
#include "main.h"
#include "message.h"
#include "chat_node.h"

// global variables
pthread_mutex_t msgMutex;

// arguments for thread
typedef struct argStruct{
    messageDataType *message;
    chatNodeDataType *node;
}argDataType;

// function prototypes
void *handleClient(void *args);

int read_complete(int socket, void* buffer, unsigned int size);

void *distrubuteMessage(void *args);

#endif // CLIENTHANDLER