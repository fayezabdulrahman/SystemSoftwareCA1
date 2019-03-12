#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include "daemonServer.h"

void main()
{
    mqd_t mq;
    struct mq_attr queue_attribute;
    char buffer [1024];

    /* set queue attributes */

    queue_attribute.mq_flags = 0;
    queue_attribute.mq_maxmsg = 10;
    queue_attribute.mq_msgsize = 1024;
    queue_attribute.mq_curmsgs = 0;

    // create queue
    mq = mq_open("/test_Queue",O_CREAT | O_RDONLY, 0644, &queue_attribute);

    ssize_t bytes_read;

    /* receive message */
    bytes_read = mq_receive(mq,buffer,1024,NULL);
    buffer[bytes_read] = '\0';

    // close 
    mq_close(mq);
    mq_unlink("/test_Queue");
    
    
}


