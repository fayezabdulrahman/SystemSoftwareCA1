#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h> // for logging to varlog
#include <sys/stat.h> // for permissions
#include <fcntl.h> // for permissions
#include <string.h> // for comparing string
#include <stdbool.h> // for boolean
#include "daemonServer.h"


// define functions
int actuallyBackingUp();

void BackUp()
{
    
    int result;
    char readbuffer[100];
    int fd[2],des,nbytes,target,pid;

    // create the pipe
    if(pipe(fd) == -1)
    {
        perror("Error init Fork 1");
        exit(EXIT_FAILURE);
    }

    // create a child process 
    if((pid = fork()) == -1)
    {
        perror("Error init Fork 1");
        exit(EXIT_FAILURE);
    }

    if(pid > 0) // this is the parent
    {
        // Send no output, closefd[1]
        close(fd[1]);

        // Get input from the pipe via read
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        syslog(LOG_INFO, readbuffer);
        exit (0);
        
    }
    else
    {
        syslog(LOG_INFO,"Inside Backup Process");

        // Take no input, close fd[0] (READ)
        close(fd[0]);
        int ReturnedLockValue = LockIt(); // locking file before backing up
        if(ReturnedLockValue == -1)
        {
            syslog(LOG_INFO,"Locking Failed");
        }
        else
        {
            syslog(LOG_INFO,"Locking Successful");
            syslog(LOG_INFO,"Calling function to actually backup");

            int ReturnedBackupValue = actuallyBackingUp();

            if(ReturnedBackupValue == -1)
            {
                UnLockFile(); // unlock file incase it fails
                char message[] = "Backup FAILED Refer to logs";
                write(fd[1],message, (strlen(message)+1));
                
            }
            else
            {
                UnLockFile(); // unlock file when everything works
                char message[] = "Backup Successfully completed";
                write(fd[1],message, (strlen(message)+1));
            }
        }

    }
}


int actuallyBackingUp()
{
    syslog(LOG_INFO, "INSIDE ACTUALLYBACKINGUP");

    int checker = system("cp -a -u /var/www/html/intranet/. /var/www/html/intranetBackup/"); // the -u operator only copies changed content in the file 
    return checker;

}

int LockIt()
{
    syslog(LOG_INFO, "LOCKING FILE Before backing up!");
    // change file perimissions to read for user, read for group, read for others, no one allowed to write to it
    //chmod(LIVE,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    int result = system("chmod 444 /var/www/html/intranet");

    return result;

}

void UnLockFile()
{
    syslog(LOG_INFO, "Unlocking FILE After backing/transfer up!");
    // change file perimissions to read write execute for user read for group, read for others
    //chmod(LIVE,S_IRWXU|S_IWUSR|S_IRGRP|S_IROTH);
    system("chmod 755 /var/www/html/intranet"); 
    system("chmod 700 /var/www/html/intranetBackup");
}