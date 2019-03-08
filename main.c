#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <stdbool.h>
#include "daemonServer.h"

//Global variables
int MIDNIGHT = 00;
int main (int argc, char *argv[] )
{
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
        

        // THis if statement checks if time is midnight, if it is, it will call the BACKUP function which will fork a backup Process and copy files over
        if(systemHour == 01)
        {
            bool IsFileLocked = LockFile(); // CALL Lock file function to check if file is locked or not.. this function is inside BackUp.c
            if(IsFileLocked = true )
            {
                syslog(LOG_INFO,"File already locked, Can begin backup");
                // call back up function which will go into BackUp.c and fork a new child to COPY files over.
                int result = BackUp();
                if (result == 1)
                {
                    syslog(LOG_INFO,"Successfully backed up website!");
                }
                else
                {
                    syslog(LOG_INFO,"FAILED to backup website!");
                }
                    
            } // end if
               
        } // en big iff
        else
        {
            syslog(LOG_INFO,"cant back up now");
        }
        
            
    } // Daemon loop!
        
    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();

    return 0;
    
}