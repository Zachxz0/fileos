.PHONY: all clean

all : root
	echo Designed by zoson

root : root.o Fileos.o Command.o Fileoperator.o User.o DiskFormat.o
	g++ -o root root.o Fileos.o Command.o Fileoperator.o User.o DiskFormat.o

root.o : root.cpp Fileos.h
	g++ -o root.o -c root.cpp

Fileos.o :Fileos.cpp Fileos.h Command.h Fileoperator.h User.h DiskFormat.cpp Node.h
	g++ -o Fileos.o -c Fileos.cpp

Command.o: Command.cpp Command.h Fileoperator.h
	g++ -o Command.o -c Command.cpp

Fileoperator.o: Fileoperator.cpp Fileoperator.h Fileos.h User.h DiskFormat.cpp Node.h
	g++ -o Fileoperator.o -c Fileoperator.cpp

User.o:User.cpp User.h Node.h
	g++ -o User.o -c User.cpp

DiskFormat.o:DiskFormat.h DiskFormat.cpp Node.h
	g++ -o DiskFormat.o -c DiskFormat.cpp
clean:
	rm -f root *.o *.fs
