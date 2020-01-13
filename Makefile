myapp:	Administrator.o Record-Keeper.o
	gcc -o myapp Administrator.o Record-Keeper.o

Administrator.o: Administrator.c
	gcc -c Administrator.c

Record-Keeper.o: Record-Keeper.c
	gcc -c Record-Keeper.c

 
