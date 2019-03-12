#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include "daemonServer.h"

void main(char message[])
{
    sendResponse(message);   
}

void sendResponse(char message[])
{
    mqd_t mq;
    char buffer[1024];

    // open the message queue
    mq = mq_open("/test_Queue",O_WRONLY);

    memset(buffer,0,1024);
    mq_send(mq,message,1024,0);

    // close 
    mq_close(mq);
}