all: DCEL.o

DCEL.o : DCELtmp.o Edge.o Point.o SimplePolygon.o Vector.o ../AVLTree/AVLTree.o
	ld -o DCEL.o -r DCELtmp.o Edge.o Point.o SimplePolygon.o Vector.o ../AVLTree/AVLTree.o

../AVLTree/AVLTree.o : 
	(cd ../AVLTree;make all)

DCELtmp.o : DCEL.cpp DCEL.h Edge.h Point.h SimplePolygon.h Vector.h ../AVLTree/AVLTree.h
	g++ -o DCELtmp.o -c DCEL.cpp -std=c++17

SimplePolygon.o : SimplePolygon.h Edge.h SimplePolygon.cpp
	g++ -o SimplePolygon.o -c SimplePolygon.cpp

Vector.o : Vector.cpp Vector.h Point.h Edge.h
	g++ -o Vector.o -c Vector.cpp

Edge.o : Edge.cpp Edge.h Point.h
	g++ -o Edge.o -c Edge.cpp -std=c++17

Point.o : Point.cpp Point.h
	g++ -o Point.o -c Point.cpp

clean :
	rm -r ./*.o
