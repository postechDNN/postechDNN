TARGET = *.o *.exe

all: subdirs main.exe 

SUBDIRS = DCEL
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

main.exe : main.o C_Subdivision.o WC_region.o CS_valid_check.o
	g++ -o main.exe main.o C_Subdivision.o WC_region.o CS_valid_check.o ./DCEL/DCEL_lib.a

main.o : main.cpp C_Subdivision.h DCEL/DCEL.h DCEL/point.h
	g++ -o main.o -c main.cpp -std=c++17

C_Subdivision.o : C_Subdivision.cpp Disjoint_Set.h
	g++ -o C_Subdivision.o -c C_Subdivision.cpp -std=c++17

WC_region.o : WC_region.cpp DCEL/DCEL.h
	g++ -o WC_region.o -c WC_region.cpp -std=c++17

CS_valid_check.o : CS_valid_check.cpp WC_region.h DCEL/DCEL.h
	g++ -o CS_valid_check.o -c CS_valid_check.cpp -std=c++17

CLEANSUBDIRS = $(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEANSUBDIRS)
clean: $(CLEANSUBDIRS)
	del $(TARGET)
$(CLEANSUBDIRS): clean-%:
	$(MAKE) -C $* clean