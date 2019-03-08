#ifndef DAEMONSERVER_H_
#define DAEMONSERVER_H_
#include <stdbool.h> // used for boolean because C99 takes needs this header file

void RunDaemonServer();
int BackUp();
bool LockFile();
void UnLockFile();

#endif // Ending DAEMONSERVER_H_