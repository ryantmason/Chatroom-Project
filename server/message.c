#include "message.h"


/* ******************************************************************** */
/* builds/ initializes the message structure and returns                */
/* ******************************************************************** */
messageFlags buildMessage(messageDataType *message, char *name, char *note)
{
    // set the member values
    strcpy(message->name, name);
    strcpy(message->messageNote, note);

    if( strcmp(message->messageNote , "JOIN") == STR_EQ )
        {
            message->chatFlag = JOIN;
        }
        else if( strcmp(message->messageNote , "LEAVE") == STR_EQ )
        {
            message->chatFlag = LEAVE;
        }
        else if( strcmp(message->messageNote , "SHUTDOWN") == STR_EQ )
        {
            message->chatFlag = SHUTDOWN;
        }
        else if( strcmp(message->messageNote , "SHUTDOWN ALL") == STR_EQ )
        {
            message->chatFlag = SHUTDOWN_ALL;
        }
        else
        {
            message->chatFlag = NOTE;    
        }

    // return new message node
    return message->chatFlag;
}

/* ******************************************************************** */
/* Sends the message to all clients                                     */
/* ******************************************************************** */
void sendMessage(int socket, messageDataType *message)
{
    // initial variables
    char flagBuffer[4];
    sprintf(flagBuffer, "%d", message->chatFlag);

    char name[16] = {0};
    strncpy( name, message->name, 16 );

    char note[64] = {0};
    strncpy( note, message->messageNote, 64 );
    
    //send message over socket using the following order:
    // read from the the socket
    // structure for read/write (in order): 
    // type flag (int/enum)
    // name (up to 16 chars, stopped by null char)
    // note (up to 64 chars, stopped by null char)
    write( socket, &flagBuffer, 4 );

    // write the name
    write( socket, &name, 16 );

    // write the note
    write( socket, &note, 64 );
    
}

    

