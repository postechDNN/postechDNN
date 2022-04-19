all: test.exe



test.exe : main.o DCEL.o Edge.o Point.o Vector.o
	g++ -o main.exe main.o DCEL.o Edge.o Point.o Vector.o

main.o : main.cpp AVLTree.h
	g++ -o main.o -c main.cpp -std=c++17

DCEL.o : DCEL.cpp DCEL.h Edge.h Point.h Vector.h AVLTree.h
	g++ -o DCEL.o -c DCEL.cpp -std=c++17

Vector.o : Vector.cpp Vector.h Point.h Edge.h
	g++ -o Vector.o -c Vector.cpp -std=c++17

Edge.o : Edge.cpp Edge.h Point.h
	g++ -o Edge.o -c Edge.cpp -std=c++17

Point.o : Point.cpp Point.h
	g++ -o Point.o -c Point.cpp

clean :
	del *.o main.exe
