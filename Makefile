myprog :	daemonServer.o BackUp.o main.o
	gcc -o myprog daemonServer.o BackUp.o main.o

main.o :	main.c
	gcc -c main.c

daemonServer.o :	daemonServer.c daemonServer.h
	gcc -c daemonServer.c

BackUp.o:	BackUp.c daemonServer.h
	gcc -c BackUp.c

clean :
	rm myprog daemonServer.o main.o BackUp.o