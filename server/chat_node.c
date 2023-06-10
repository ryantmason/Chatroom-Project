#include "chat_node.h"

// add new node to end of linked list
/* ******************************************************************** */
/* adds client in linked list and updates head pointer                  */
/* ******************************************************************** */
void addClient(chatNodeDataType** headClient, char *ip, int port, char clientName[])
{
    chatNodeDataType* newClient = (chatNodeDataType*) malloc(sizeof(chatNodeDataType));

    // initialize variables
    strcpy(newClient->ip, ip);
    newClient->port = port;
    strcpy(newClient->clientName, clientName);
    // assign next node to null
    newClient->nextNode = NULL;

    chatNodeDataType *current = *headClient;

    // check if the LL is empty
    if( *headClient == NULL )
    {
        *headClient = newClient;
    }
    else
    {

        // otherwise, loop through the LL
        // loop through LL until end (check if next node is NULL)
        while( current->nextNode != NULL )
        {
            // increment node
            current = current->nextNode;
            // end loop
        }

        // add node as next node
        current->nextNode = newClient;
    }
}


/* ******************************************************************** */
/*                      clears chat list                                */
/* ******************************************************************** */
void clearList(chatNodeDataType *headClient)
{
    // initialize variables
    chatNodeDataType *currentClient = headClient;
    chatNodeDataType *temp;

    // loop traverse through LL until the end 
    while( currentClient != NULL )
    {
        // set the temp node to the current head
        temp = currentClient;

        // set the head to the next node (incrementing here)
        currentClient = currentClient->nextNode;

        // free the temp node
        free(temp);

    // end loop
    }
}

/* ******************************************************************** */
/* removes client from Linked List                                      */
/* ******************************************************************** */
void removeClient(chatNodeDataType** headClient, char *ip)
{
    // initial variables/ functions
        chatNodeDataType *currentNode = *headClient;
        chatNodeDataType *previousNode = NULL;
        chatNodeDataType *tempNode = NULL;

        // traverse(loop) entire node LL 
        while(currentNode != NULL)
           {
            // check if current node has the 
            if(strcmp(currentNode->ip,ip) == STR_EQ)
                {
                // check if there is a previous block
                if(previousNode != NULL)
                    {
                     // point previous block to current's next block
                     previousNode->nextNode = currentNode->nextNode;                   
                    }
                // otherwise, next block is head
                else
                    {
                     // set next block to head
                     *headClient = currentNode->nextNode; 
                              
                    }     
                    // set the temp block to the current
                    tempNode = currentNode;          
                }

            // free the temp node (was the current)
            if(tempNode != NULL)
                {
                    //free block
                    free(tempNode);

                    // reset pointer
                    tempNode = NULL;
                }
            // otherwise, store previous
            else
                {
                    previousNode = currentNode;
                }

            // increment current block
            currentNode = currentNode->nextNode;

        // end loop
           }
}
