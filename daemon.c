#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int backup();
int main()
{
    syslog(LOG_INFO,"Daemon Server Running");
    char readbuffer[100];
    char childbuff[100];
    int fd[2],des,nbytes,target;
    // Create a fork child process
    int pid = fork();

    if(pid> 0)
    {
        // if PID > 0 : this is the parent
        // this process performs printf and finishes

        printf("Parent process DIED\n");
        sleep(1);
        exit(EXIT_SUCCESS);
    }
    else if( pid == 0)
    {
        // this is a child process
        printf("Child Process");

        // creating DAEMON SERVER FROM HERE ONWARDS
        int sid;
        sid = setsid();
        openlog("DAEMON LOG", LOG_PID|LOG_CONS, LOG_USER);
        if(sid< 0)
        {
            exit(EXIT_FAILURE);
        }
        umask(0);

        if(chdir("/") < 0) 
        {
            exit(EXIT_FAILURE);
        }

        int x;
        for(x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
            close(x);
        }

        int i = 0;

        while(1)
        {
            if( i == 0)
            {
                i =1;
                // create the pipe
                pipe(fd);

                // create a child process 
                int backupPID;
                backupPID = fork();

                if(backupPID > 0) // this is the parent
                {
                    // Send no output, closefd[1]
                    close(fd[1]);

                    // Get input from the pipe via read
                    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                    //syslog(LOG_INFO, readbuffer);
                    

                }
                else
                {
            
                    syslog(LOG_INFO,"Inside Backup Child Process");
                    
                    // Take no input, close fd[0] (READ)
                    close(fd[0]);

                    // CALL BACKUP Method
                    int result = backup();
                    if(result == 1)
                    {
                        syslog(LOG_INFO,"CP WORKED MA NIGGAA!");
                    }
                    else
                    {
                        syslog(LOG_INFO,"CP FAILED TO COPY!");
                    }

                    exit(0);
                    
                }
            }
            
        }

        closelog();
    }
    return 0;
}
