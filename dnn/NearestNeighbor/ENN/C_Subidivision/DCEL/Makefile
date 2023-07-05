all: DCEL_lib.a DCEL_operation.o DCEL_IO.o DCEL.o Edge.o Point.o Vector.o Polygon.o

DCEL_lib.a : DCEL_operation.o DCEL_IO.o DCEL.o Edge.o Point.o Vector.o Polygon.o
	ar rscv DCEL_lib.a DCEL_operation.o DCEL_IO.o DCEL.o Edge.o Point.o Vector.o Polygon.o

DCEL_IO.o : DCEL_IO.cpp DCEL.h 
	g++ -o DCEL_IO.o -c DCEL_IO.cpp -std=c++17

DCEL.o : DCEL.cpp DCEL.h Edge.h Point.h Vector.h AVLTree.h DCEL_operation.h
	g++ -o DCEL.o -c DCEL.cpp -std=c++17

DCEL_operation.o : DCEL_operation.cpp DCEL.h Vector.h
	g++ -o DCEL_operation.o -c DCEL_operation.cpp -std=c++17

Vector.o : Vector.cpp Vector.h Point.h Edge.h
	g++ -o Vector.o -c Vector.cpp -std=c++17

Edge.o : Edge.cpp Edge.h Point.h
	g++ -o Edge.o -c Edge.cpp -std=c++17

Point.o : Point.cpp Point.h
	g++ -o Point.o -c Point.cpp -std=c++17

Polygon.o : Polygon.cpp Polygon.h
	g++ -o Polygon.o -c Polygon.cpp -std=c++17

clean :
	del *.o *.a
