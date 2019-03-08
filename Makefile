myprog :	daemonServer.o BackUp.o
	gcc -o myprog daemonServer.o BackUp.o

daemonServer.o :	daemonServer.c daemonServer.h
	gcc -c daemonServer.c

BackUp.o:	BackUp.c daemonServer.h
	gcc -c BackUp.c

clean :
	rm myprog daemonServer.o BackUp.o