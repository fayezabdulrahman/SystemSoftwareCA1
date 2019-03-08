#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//global variable
char LOCKINGPERMISSION[] = "r-r--r--";
//define function
int getChmod(const char *path);
void main()
{
    //printf("%o\n",getChmod(FILE));

    
    //struct stat buffer;
    //int  status;

    //status = stat("/home/fayez/CA/test.txt", &buffer);
    //printf("%i\n",status);


    /*
    The first expression conditionalExpression is evaluated first. This expression evaluates to 1 if it's true and evaluates to 0 if it's false.
    If conditionalExpression is true, expression1 is evaluated.
    If conditionalExpression is false, expression2 is evaluated.
    */

    printf("Before changing permissions\n");
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    if(stat("/home/fayez/CA/test.txt", &st) == 0)
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
        printf("%s\n",modeval);
    }
    else
    {
        printf("Permission not set!");
    }

    if(strcmp(modeval,LOCKINGPERMISSION) == 0)
    {
        printf("File is locked boyyyyyyyyyyyy\n");
    }
    else
    {
        printf("LOCK FILE FIRST!\n");
    }
    
/*
    // change file perimissions
    chmod("/home/fayez/CA/test.txt",S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

    printf("After changing permissions\n");
    if(stat("/home/fayez/CA/test.txt", &st) == 0)
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
        printf("%s\n",modeval);
    }
    else
    {
        printf("Permission not set!");
    }
*/

}

int lockfile()
{
    struct stat st;
    char *modeval = malloc(sizeof(char) * 9 + 1);
    if(stat("/home/fayez/CA/test.txt", &st) == 0)
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
        printf("%s\n",modeval);
    }
    else
    {
        printf("Permission not set!");
    }

    if(strcmp(modeval,LOCKINGPERMISSION) == 0)
    {
        printf("File is locked boyyyyyyyyyyyy\n");
    }
    else
    {
        printf("LOCK FILE FIRST!\n");
    }
}

int getChmod(const char *path)
{
    struct stat ret;

    if (stat(path, &ret) == -1) 
    {
        return -1;
    }

    return (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|/*owner*/
        (ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|/*group*/
        (ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);/*other*/
}
