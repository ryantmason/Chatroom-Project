// header file for client main
#ifndef MAIN_H
#define MAIN_H

//packages 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "receiver_handler.h"
#include "sender_handler.h"
#include "message.h"
#include "properties.h"

// global variables
char name[16];
char port[6];

#endif // main