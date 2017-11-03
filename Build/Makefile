CFLAGS = -c -fPIC -ansi -pedantic -Wall -Werror -O0

All: Clean Build Files Libs

Libs:
	gcc ../Clean/Main/*.o -o ../Run/Main
	gcc -shared ../Clean/Libs/*.o -o ../Run/libChaos.so
	ar rcs ../Run/libChaos.a ../Clean/Libs/*.o

Build: Main HashMap Memory Type

Main:
	gcc $(CFLAGS) ../Source/Main.c -I../Headers -o ../Clean/Main/Main.o

HashMap:
	gcc $(CFLAGS) ../Source/HashMap.c -I../Headers -o ../Clean/Main/HashMap.o

Memory:
	gcc $(CFLAGS) ../Source/Memory.c -I../Headers -o ../Clean/Main/Memory.o
	cp ../Clean/Main/Memory.o ../Clean/Libs/Memory.o

Type:
	gcc $(CFLAGS) ../Source/Type.c -I../Headers -o ../Clean/Main/Type.o

Files: Num

Num:
	gcc $(CFLAGS) ../Source/Num.c -I../Headers -o ../Clean/Libs/Num.o

Clean:
	rm -rf ../Clean/Main/*
	rm -rf ../Clean/Libs/*
	rm -rf ../Run/*
