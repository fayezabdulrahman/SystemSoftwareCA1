#ifndef DAEMONSERVER_H_
#define DAEMONSERVER_H_
#include <stdbool.h> // used for boolean because C99 takes needs this header file

void RunDaemonServer();
void BackUp();
void UnLockFile();
void LockIt();

#endif // Ending DAEMONSERVER_H_