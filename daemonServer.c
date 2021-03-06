#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <stdbool.h>
#include "daemonServer.h"
#include <string.h>
#include <sys/types.h>

//Global variables
int MIDNIGHT = 00;
bool backedUp = false;
void RunDaemonServer()
{
    //Create a child process
    int pid;

    if((pid = fork()) == -1)
    {
        perror("Error init Fork 1");
        exit(EXIT_FAILURE);
    }

    if(pid > 0)
    {
        // this is the parent
        // this will perform a printf and finish
        printf("Daemon Server Initiated...\n");
        exit(EXIT_SUCCESS); // KILL THE parent
    }
    else if(pid == 0)
    {
        FILE *somefile;
        somefile = fopen("/tmp/daemonPID.txt","w");
        //text
        fprintf(somefile, "%d", getpid());

        fclose(somefile);

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


int main (int argc, char *argv[] )
{
    /*
    If an argument is passed we check if its backup, and then we can force back up
    */
    
    if(argc >1)
    {
        char argumentPassed[100];
        strcpy(argumentPassed,argv[1]);
        if(strcmp(argumentPassed,"backup") == 0)
        {
            syslog(LOG_INFO,"Forcing backup..");

            BackUp();

            exit(0);            
        }
        if(strcmp(argumentPassed,"transfer") == 0)
        {
            syslog(LOG_INFO,"Forcing transfer..");

            TransferFiles(); 

            exit(0);   
        }
    }

    // if no argument is passed, just run the daemon server, because our init script
    // already checks if there is an instance running of the file. 

    // calling daemon Server to start
    RunDaemonServer();

    while (1) // THis while loop to keep DAEMON Server running after its been called 
    {
        //INSERT DAEMON CODE HERE
        sleep(2);
        syslog(LOG_INFO,"Inside Daemon Server");

        // create currentTIme variable
        time_t currentTime;
        time(&currentTime);

        // add it to tm strucutre that gets local time
        struct tm *myTime = localtime(&currentTime);
        
        // get system time and assign it to variable
        int systemHour = myTime->tm_hour;
        int systemMinute = myTime->tm_min;

        while(backedUp == false)
        {
            if(systemHour == 23 ) // essentially it would be if systemHour == MIDNIGHT
            {
            
                backedUp = true; // set backup to true so we don't get stuck in a loop
                
                //  call the Tranferfiles() function which will call our audifiles.c program
                TransferFiles(); 
                syslog(LOG_INFO,"Calling function to create backup...");
                // call the BackUp function inside BackUp.c program which will lock files, and back up and transfer
                BackUp();
            
            } 
        }          
    } // Daemon loop!
        
    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();

    return 0;
    
    
}