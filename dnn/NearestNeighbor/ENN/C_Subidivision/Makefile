TARGET = *.o *.exe

all: subdirs main.exe 

SUBDIRS = DCEL Testcode PointLocation
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

main.exe : main.o C_Subdivision.o WC_region.o CS_Free.o APX_wavefront.o WF_propagation.o SPM.o Testcode/test_csdiv_pts/test_csdiv_pts.h
	g++ -o main.exe main.o C_Subdivision.o WC_region.o CS_Free.o APX_wavefront.o WF_propagation.o SPM.o ./DCEL/DCEL_lib.a ./Testcode/test_lib.a ./PointLocation/Location_lib.a

main.o : main.cpp C_Subdivision.h WF_propagation.h SPM.h DCEL/DCEL.h DCEL/point.h
	g++ -o main.o -c main.cpp -std=c++17

C_Subdivision.o : C_Subdivision.cpp Disjoint_Set.h
	g++ -o C_Subdivision.o -c C_Subdivision.cpp -std=c++17

WC_region.o : WC_region.cpp DCEL/DCEL.h
	g++ -o WC_region.o -c WC_region.cpp -std=c++17

CS_Free.o : CS_Free.cpp PointLocation/Location.h DCEL/DCEL.h DCEL/Polygon.h C_Subdivision.h
	g++ -o CS_Free.o -c CS_Free.cpp -std=c++17

APX_wavefront.o : APX_wavefront.cpp
	g++ -o APX_wavefront.o -c APX_wavefront.cpp -std=c++17

WF_propagation.o : WF_propagation.cpp DCEL/DCEL.h APX_wavefront.h CS_Free.h
	g++ -o WF_propagation.o  -c WF_propagation.cpp -std=c++17

SPM.o : SPM.cpp DCEL/DCEL.h WF_propagation.h
	g++ -o SPM.o  -c SPM.cpp -std=c++17


CLEANSUBDIRS = $(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEANSUBDIRS)
clean: $(CLEANSUBDIRS)
	del $(TARGET)
$(CLEANSUBDIRS): clean-%:
	$(MAKE) -C $* clean