all: test_csdiv_pts.o CS_valid_check.o 

CS_valid_check.o : CS_valid_check.cpp CS_valid_check.h ../../WC_region.h ../../DCEL/DCEL.h
	g++ -o ../CS_valid_check.o -c CS_valid_check.cpp -std=c++17

test_csdiv_pts.o : test_csdiv_pts.cpp ../../DCEL/Point.h ../../C_Subdivision.h CS_valid_check.h
	g++ -o ../test_csdiv_pts.o -c test_csdiv_pts.cpp -std=c++17