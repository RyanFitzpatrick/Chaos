CFLAGS = -c -fPIC -ansi -pedantic -Wall -Werror -O0

All: Clean Build Files Libs Run

Run:
	gcc ../Clean/Main/*.o -o ../Run/Main

Libs:
	gcc -shared ../Clean/Libs/*.o -o ../Run/libChaos.so
	ar rcs ../Run/libChaos.a ../Clean/Libs/*.o

Build: Main HashMap Parser ParseStack ParseTree Symbol SymbolTable Type

Main:
	gcc $(CFLAGS) ../Source/Main.c -I../Headers -o ../Clean/Main/Main.o

HashMap:
	gcc $(CFLAGS) ../Source/HashMap.c -I../Headers -o ../Clean/Main/HashMap.o

Parser:
	gcc $(CFLAGS) ../Source/Parser.c -I../Headers -o ../Clean/Main/Parser.o

ParseStack:
	gcc $(CFLAGS) ../Source/ParseStack.c -I../Headers -o ../Clean/Main/ParseStack.o

ParseTree:
	gcc $(CFLAGS) ../Source/ParseTree.c -I../Headers -o ../Clean/Main/ParseTree.o

Symbol:
	gcc $(CFLAGS) ../Source/Symbol.c -I../Headers -o ../Clean/Main/Symbol.o

SymbolTable:
	gcc $(CFLAGS) ../Source/SymbolTable.c -I../Headers -o ../Clean/Main/SymbolTable.o

Type:
	gcc $(CFLAGS) ../Source/Type.c -I../Headers -o ../Clean/Main/Type.o

Files: Num

Num:
	gcc $(CFLAGS) ../Source/Num.c -I../Headers -o ../Clean/Libs/Num.o

Clean:
	rm -rf ../Clean/Main/*
	rm -rf ../Clean/Libs/*
	rm -rf ../Run/*
