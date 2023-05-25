TARGET = *.o *.exe

all: subdirs main.exe 

SUBDIRS = DCEL
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

main.exe : main.o C_Subdivision.o
	g++ -o main.exe main.o C_Subdivision.o ./DCEL/DCEL_lib.a

main.o : main.cpp C_Subdivision.h
	g++ -o main.o -c main.cpp -std=c++17

C_Subdivision.o : C_Subdivision.cpp Disjoint_Set.h
	g++ -o C_Subdivision.o -c C_Subdivision.cpp -std=c++17

CLEANSUBDIRS = $(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEANSUBDIRS)
clean: $(CLEANSUBDIRS)
	del $(TARGET)
$(CLEANSUBDIRS): clean-%:
	$(MAKE) -C $* clean