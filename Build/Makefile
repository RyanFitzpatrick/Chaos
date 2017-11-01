CC = gcc
CFLAGS = -c -fPIC -ansi -pedantic -Wall -Werror -O0

All: Clean Main Memory Num Libs

Libs:
	gcc ../Clean/Main/*.o -o ../Run/Main
	gcc -shared ../Clean/Libs/*.o -o ../Run/libChaos.so
	ar rcs ../Run/libChaos.a ../Clean/Libs/*.o

Main:
	$(CC) $(CFLAGS) ../Source/Main.c -I../Headers -o ../Clean/Main/Main.o

Memory:
	$(CC) $(CFLAGS) ../Source/Memory.c -I../Headers -o ../Clean/Main/Memory.o
	cp ../Clean/Main/Memory.o ../Clean/Libs/Memory.o

Num:
	$(CC) $(CFLAGS) ../Source/Num.c -I../Headers -o ../Clean/Libs/Num.o

Clean:
	rm -rf ../Clean/Main/*
	rm -rf ../Clean/Libs/*
	rm -rf ../Run/*
