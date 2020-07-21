All : DCELdeepcopy.o

Triangulation.o : Triangulation_tmp.o ../BBST/SimpleBBST.o
	#(cd ../DCEL;make all)
	(cd ../BBST;make all)
	ld -r -o Triangulation.o Triangulation_tmp.o ../BBST/SimpleBBST.o

DCELdeepcopy.o : DCELdeepcopy.cpp DCELdeepcopy.h
	g++ -o DCELdeepcopy.o -c DCELdeepcopy.cpp


