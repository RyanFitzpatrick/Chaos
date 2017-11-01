All: Clean Num Libs

Libs:
	gcc -shared ../Clean/Libs/*.o -o ../Run/libChaos.so
	ar rcs ../Run/libChaos.a ../Clean/Libs/*.o

Num:
	gcc -c -fPIC -ansi -pedantic -Wall -Werror ../Source/Num.c -I../Headers -o ../Clean/Libs/Num.o

Clean:
	rm -rf ../Clean/Main/*
	rm -rf ../Clean/Libs/*
	rm -rf ../Run/*
