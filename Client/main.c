// client side main
#include "main.h"

// Nicholas Henderson, Ryan Mason

/* ******************************************************************** */
/* Reads properties, starts 2 threads which implement sender, receiver  */
/* ******************************************************************** */
int main(int argc, char *argv[])
{
    // initialize all variables 
    char* properties_file = "test.properties";

    Properties* properties;
    char* portKey = "MY_PORT";
    char* nameKey = "NAME";

    pthread_t send_thread, receieve_thread;

    properties = property_read_properties(properties_file);

    strcpy(port, property_get_property(properties, portKey));
    strcpy(name, property_get_property(properties, nameKey));

    pthread_create(&receieve_thread, NULL, handleServer, NULL);
    pthread_detach(receieve_thread);
    
    pthread_create(&send_thread, NULL, handleMessage, NULL);
    pthread_join(send_thread, NULL);

    return 0;
}