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
        printf("Parent Process Terminating\n");
        exit(EXIT_SUCCESS); // KILL THE parent
    }
    else if(pid == 0)
    {
        FILE *somefile;
        somefile = fopen("/home/fayezrahman/Desktop/CA1/SystemSoftwareCA1/daemonPID.txt","w");
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
    // this here checks if ./myprog stop is passed and it kills the daemon running
    if(argc >1)
    {
        char argumentPassed[100];
        strcpy(argumentPassed,argv[1]);
        if(strcmp(argumentPassed,"stop") == 0)
        {
            syslog(LOG_INFO,"Stopping dameon");

            system("sudo echo "" > daemonPID.txt"); // clear the PID from the file 

            int checker = system("sudo killall myprog"); // the -u operator only copies changed content in the file
            if(checker ==! -1)
            {
                exit(0);
            }
        }

        if(strcmp(argumentPassed,"force") == 0)
        {
            syslog(LOG_INFO,"Forcing backup..");

            BackUp();

            exit(0);            
        }

        if(strcmp(argumentPassed,"start") == 0)
        {
            FILE *fp;
            fp = fopen("/home/fayezrahman/Desktop/CA1/SystemSoftwareCA1/daemonPID.txt","r");

            fseek (fp, 0, SEEK_SET);
            
            if (ftell(fp) == 0)
            {
                //file empty
                syslog(LOG_INFO,"FACK OFF!");
            }
            else
            {
                syslog(LOG_INFO,"I can run a new instance.......");
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
                        if(systemHour == 14 )
                        {
                        
                            backedUp = true;
                            //syslog(LOG_INFO,"Calling Function to create log files for today..");
                            runLogfilesForToday(); 
                            syslog(LOG_INFO,"Calling function to create backup...");
                            BackUp();
                        
                        } 
                    }          
                } // Daemon loop!
                    
                syslog (LOG_NOTICE, "First daemon terminated.");
                closelog();

                return 0;
            }
                 
    /*
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
            if(systemHour == 14 )
            {
            
                backedUp = true;
                //syslog(LOG_INFO,"Calling Function to create log files for today..");
                runLogfilesForToday(); 
                syslog(LOG_INFO,"Calling function to create backup...");
                BackUp();
            
            } 
        }          
    } // Daemon loop!
        
    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();

    return 0;
    */
    
}