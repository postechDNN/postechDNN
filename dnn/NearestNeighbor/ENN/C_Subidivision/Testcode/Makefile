all: subdirs test_lib.a 

SUBDIRS = test_csdiv_pts test_L_infty_DT
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)	
$(SUBDIRS): 
	$(MAKE) -C $@

test_lib.a : CS_valid_check.o test_csdiv_pts.o test_L_infty_DT.o
	ar rscv test_lib.a CS_valid_check.o test_csdiv_pts.o test_L_infty_DT.o

clean :
	del *.o *.a