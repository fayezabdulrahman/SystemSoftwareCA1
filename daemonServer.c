#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

void RunDaemonServer()
{
    //Create a child process
    int pid = fork();

    if(pid > 0)
    {
        // this is the parent
        // this will perform a printf and finish
        printf("Parent Process Terminating\n");
        exit(EXIT_SUCCESS); // KILL THE parent
    }
    else if(pid == 0)
    {
        //Create the orphan process
        printf("Child process");
        syslog (LOG_NOTICE, "Daemon Server running...");

        //Elvate the orphan process to session leader
        //THis command runs the process in a new session
        if(setsid() < 0)
        {
            exit(EXIT_FAILURE);
        }

        //CAll umask() to set the file mode creation mask to 0
        umask(0);

        //Change the current working directory to root
        // this eleminates any issue of running on a monuted drive
        if(chdir("/") < 0 )
        {
            exit(EXIT_FAILURE);
        }

        // CLose all open file descriptors
        int x;
        for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
            close(x);
        }

        // Open the log file 
        openlog("Init Daemon", LOG_PID|LOG_CONS, LOG_USER);
    }
}