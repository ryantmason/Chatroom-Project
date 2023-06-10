// protect from multiple compiling
#ifndef CHATNODE_H
#define CHATNODE_H

#include <stdlib.h>
#include <string.h>

#define STR_EQ 0

// client list LL data structure
typedef struct chatNodeStruct
    {
        char ip[16];
        int port;
        char clientName[16];

        struct chatNodeStruct *nextNode;
    } chatNodeDataType;

// function prototypes
void addClient(chatNodeDataType** headClient, char *ip, int port, char clientName[]);

void clearList(chatNodeDataType *headClient);

void removeClient(chatNodeDataType** headClient, char *ip);

#endif // CHATNODE