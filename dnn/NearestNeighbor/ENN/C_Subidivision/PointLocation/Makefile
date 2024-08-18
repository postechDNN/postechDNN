all : Location_lib.a

Location_lib.a : Location.o
	ar rscv Location_lib.a Location.o

Location.o : Location.cpp Location.h ../DCEL/DCEL.h ../DCEL/DCEL_operation.h
	g++ -o Location.o -c Location.cpp -std=c++17

clean :
	del *.o *.a