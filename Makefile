myprog :	daemonServer.o BackUp.o auditfiles.o transferfiles.o
	gcc -o myprog daemonServer.o BackUp.o auditfiles.o transferfiles.o

auditfiles.o:	auditfiles.c daemonServer.h
	gcc -c auditfiles.c

daemonServer.o :	daemonServer.c daemonServer.h
	gcc -c daemonServer.c

BackUp.o:	BackUp.c daemonServer.h
	gcc -c BackUp.c

transferfiles.o:	transferfiles.c daemonServer.h
	gcc -c transferfiles.c

clean :
	rm myprog daemonServer.o BackUp.o auditfiles.o transferfiles.o