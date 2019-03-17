#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h> // for logging to varlog
#include <sys/stat.h> // for permissions
#include <fcntl.h> // for permissions
#include <string.h> // for comparing string
#include <stdbool.h> // for boolean

// define functions
int runLogs();

void runLogfilesForToday()
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
    	// Take no input, close fd[0] (READ)
        close(fd[0]);

        syslog(LOG_INFO,"Inside AuditLog Process");
        //int returnedResult = runLogs();
        syslog(LOG_INFO,"CALLING CREAT AUDITt");
        result = runLogs();

        if(result == -1)
        {
            char message[] = "Audit file failed to create.... ";
            write(fd[1],message, (strlen(message)+1));
        }
        else
        {
            char message[] = "Audit file CREATED, check directory";
            write(fd[1],message, (strlen(message)+1));
        }
    }
}


int runLogs()
{
    int checker = system("sudo ausearch -f /var/www/html/ > /home/fayezrahman/Desktop/CA1/SystemSoftwareCA1/auditdddlog.txt");

    return checker;
}