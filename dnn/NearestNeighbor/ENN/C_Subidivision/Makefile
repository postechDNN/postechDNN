all: main.exe

main.exe : main.o C_Subdivision.o
	g++ -o main.exe main.o C_Subdivision.o dcel_lib.a

main.o : main.cpp C_Subdivision.h
	g++ -o main.o -c main.cpp -std=c++17

C_Subdivision.o : C_Subdivision.cpp Disjoint_Set.h
	g++ -o C_Subdivision.o -c C_Subdivision.cpp -std=c++17

clean :
	del *.o main.exe
