all: subdirs test_lib.a 

SUBDIRS = test_csdiv_pts
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)	
$(SUBDIRS): 
	$(MAKE) -C $@

test_lib.a : CS_valid_check.o test_csdiv_pts.o
	ar rscv test_lib.a CS_valid_check.o test_csdiv_pts.o

clean :
	del *.o *.a