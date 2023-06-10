// protect from multiple compiling
#ifndef MESSAGE_H
#define MESSAGE_H

// header files
#include <string.h>
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

#define STR_EQ 0

// message data strcuture
typedef struct messageDataStruct
    {
        int chatFlag;
        char name[16];
        char messageNote[64];
    } messageDataType;

// enum for message flags
typedef enum 
    {
        NOTE,
        JOIN,
        LEAVE,
        SHUTDOWN,
        SHUTDOWN_ALL
    }messageFlags;

// prototypes
messageFlags buildMessage(messageDataType *message, char *name, char *note);

void sendMessage(int socket, messageDataType *message);

#endif // MESSAGE
