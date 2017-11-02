CFLAGS = -c -fPIC -ansi -pedantic -Wall -Werror -O0

All: Clean Main Memory Num Libs

Libs:
	gcc ../Clean/Main/*.o -o ../Run/Main
	gcc -shared ../Clean/Libs/*.o -o ../Run/libChaos.so
	ar rcs ../Run/libChaos.a ../Clean/Libs/*.o

Main:
	gcc $(CFLAGS) ../Source/Main.c -I../Headers -o ../Clean/Main/Main.o

Memory:
	gcc $(CFLAGS) ../Source/Memory.c -I../Headers -o ../Clean/Main/Memory.o
	cp ../Clean/Main/Memory.o ../Clean/Libs/Memory.o

Num:
	gcc $(CFLAGS) ../Source/Num.c -I../Headers -o ../Clean/Libs/Num.o

Clean:
	rm -rf ../Clean/Main/*
	rm -rf ../Clean/Libs/*
	rm -rf ../Run/*
