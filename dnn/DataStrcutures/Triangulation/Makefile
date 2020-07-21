All : Triangulation.o

Triangulation.o : Triangulation_tmp.o ../BBST/SimpleBBST.o ../DCEL/DCEL.o
	ld -r -o Triangulation.o Triangulation_tmp.o ../BBST/SimpleBBST.o ../DCEL/DCEL.o

../DCEL/DCEL.o :
	(cd ../DCEL;make all)
	
../BBST/SimpleBBST.o :
	(cd ../BBST;make all)

Triangulation_tmp.o : Triangulation.cpp Triangulation.h DCELdeepcopy.h
	g++ -o Triangulation_tmp.o -c Triangulation.cpp -std=c++17

DCELdeepcopy.o : DCELdeepcopy.cpp DCELdeepcopy.h
	g++ -o DCELdeepcopy.o -c DCELdeepcopy.cpp -std=c++17



