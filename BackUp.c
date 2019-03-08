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
char LIVE[] = "/home/fayez/Desktop/CA1/SystemSoftwareCA1/test.txt";

int BackUp()
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
        //LockIt(); // locking the file before backing up

        // Take no input, close fd[0] (READ)
        close(fd[0]);
        char message[] = "Child Processing Backing up...";
        write(fd[1],message, (strlen(message)+1));
        int checker = system("cp -u /var/www/html/intranet.txt /var/www/html/live.txt"); // the -u operator only copies changed content in the file 
        
        if(checker == -1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    

}
/*
bool LockFile()
{
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    if(stat(LIVE, &st) == 0)
    {
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
        //printf("%s\n",modeval);
    }
    else
    {
        syslog(LOG_INFO,"Error with File Permission");
    }

    if(strcmp(modeval,LOCKINGPERMISSION) == 0)
    {
        return true;
    }
    else
    {
        // change file perimissions to read for user, read for group, read for others, no one allowed to write to it
        chmod(LIVE,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        return false;
    }
}
*/
void LockIt()
{
    syslog(LOG_INFO, "LOCKING FILE BABBYYYUHHH!");
    // change file perimissions to read for user, read for group, read for others, no one allowed to write to it
    //chmod(LIVE,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    system("chmod 444 /home/fayez/Desktop/CA1/SystemSoftwareCA1/test.txt"); // the -u operator only copies changed content in the file 

}

void UnLockFile()
{
    // change file perimissions to read write execute for user read for group, read for others
    //chmod(LIVE,S_IRWXU|S_IWUSR|S_IRGRP|S_IROTH);
    //system("chmod 444 /home/fayez/Desktop/CA1/SystemSoftwareCA1/test.txt"); // the -u operator only copies changed content in the file 
}
/*
int getChmod(const char *path)
{

    struct stat ret;

    if (stat(path, &ret) == -1) 
    {
        syslog(LOG_INFO,"Error with File Permission");
        return -1;
    }

    return (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)| //owner
        (ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)| //group
        (ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH); //other
} */
