TARGET = *.o *.exe

all: subdirs main.exe 

SUBDIRS = DCEL Testcode PointLocation
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

main.exe : main.o C_Subdivision.o WC_region.o CS_Free.o Testcode/test_csdiv_pts/test_csdiv_pts.h
	g++ -o main.exe main.o C_Subdivision.o WC_region.o CS_Free.o ./DCEL/DCEL_lib.a ./Testcode/test_lib.a ./PointLocation/Location_lib.a

main.o : main.cpp C_Subdivision.h DCEL/DCEL.h DCEL/point.h
	g++ -o main.o -c main.cpp -std=c++17

C_Subdivision.o : C_Subdivision.cpp Disjoint_Set.h
	g++ -o C_Subdivision.o -c C_Subdivision.cpp -std=c++17

WC_region.o : WC_region.cpp DCEL/DCEL.h
	g++ -o WC_region.o -c WC_region.cpp -std=c++17

CS_Free.o : CS_Free.cpp PointLocation/Location.h DCEL/DCEL.h DCEL/Polygon.h C_Subdivision.h
	g++ -o CS_Free.o -c CS_Free.cpp -std=c++17

CLEANSUBDIRS = $(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEANSUBDIRS)
clean: $(CLEANSUBDIRS)
	del $(TARGET)
$(CLEANSUBDIRS): clean-%:
	$(MAKE) -C $* clean