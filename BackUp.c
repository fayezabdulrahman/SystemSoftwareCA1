#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h> // for logging to varlog
#include <sys/stat.h> // for permissions
#include <fcntl.h> // for permissions
#include <string.h> // for comparing string
#include <stdbool.h> // for boolean
#include "daemonServer.h"

//global variable
char LOCKINGPERMISSION[] = "r--r--r--";


// define functions
int actuallyBackingUp();
int transferFilesToLive();
void BackUp()
{
    
    int result;
    char readbuffer[100];
    int fd[2],des,nbytes,target,pid;

    // create the pipe
    pipe(fd);

    // create a child process 
    pid = fork();

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
                syslog(LOG_INFO,"BACKING Failed");
                UnLockFile(); // unlock file incase it fails
            }
            else
            {
                syslog(LOG_INFO,"Backing up Successful");
                syslog(LOG_INFO,"Calling function to transfer files");

                int ReturnedTransferValue = transferFilesToLive();

                if(ReturnedTransferValue == -1)
                {
                    char message[] = "Backup/Transfer FAILED Refer to logs";
                    write(fd[1],message, (strlen(message)+1));
                    UnLockFile(); // unlock file incase it fails

                }
                else
                {
                    UnLockFile(); // unlock file incase it fails
                    char message[] = "Backup/Transfer fully successful";
                    write(fd[1],message, (strlen(message)+1));
                }
            }
        }

    }
}

int transferFilesToLive()
{
    syslog(LOG_INFO, "INSIDE TRANSFER FILES !!");
    int checker = system("cp -u /var/www/html/intranet/intranet.txt /var/www/html/live/live.txt"); // the -u operator only copies changed content in the file 
    return checker;
}

int actuallyBackingUp()
{
    syslog(LOG_INFO, "INSIDE ACTUALLYBACKINGUP");

    int checker = system("cp -u /var/www/html/intranet/intranet.txt /var/www/html/intranetBackup.txt"); // the -u operator only copies changed content in the file 
    return checker;

}

int LockIt()
{
    syslog(LOG_INFO, "LOCKING FILE Before backing up!");
    // change file perimissions to read for user, read for group, read for others, no one allowed to write to it
    //chmod(LIVE,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    int result = system("chmod 444 /var/www/html/intranet/intranet.txt"); // the -u operator only copies changed content in the file 

    return result;

}

void UnLockFile()
{
    syslog(LOG_INFO, "Unlocking FILE After backing/transfer up!");
    // change file perimissions to read write execute for user read for group, read for others
    //chmod(LIVE,S_IRWXU|S_IWUSR|S_IRGRP|S_IROTH);
    system("chmod 775 /var/www/html/intranet/intranet.txt"); // the -u operator only copies changed content in the file 
}