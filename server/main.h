// server side main header 
// protect from multiple compiling
#ifndef MAIN_H
#define MAIN_H

// Andrew Usvat, Nicholas Henderson, Ryan Mason
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
#include "client_handler.h"
#include "chat_node.h"
#include "properties.h"

pthread_mutex_t mutex;
chatNodeDataType *headNode;

/* Preprocessor directives */
#define SERVER_ADDR "127.0.0.1" // loopback ip address

#define FALSE 0
#define TRUE !FALSE

#define NUM_CONNECTIONS 5       // number of pending connections in the connection queue


#endif // MAIN

//gcc -pthread -Wall main.c message.c client_handler.c chat_node.c -o server