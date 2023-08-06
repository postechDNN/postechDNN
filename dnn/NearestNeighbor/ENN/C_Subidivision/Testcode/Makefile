all: subdirs test_lib.a 

SUBDIRS = test_csdiv_pts test_L_infty_DT test_L_infty_CP
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)	
$(SUBDIRS): 
	$(MAKE) -C $@

test_lib.a : CS_valid_check.o test_csdiv_pts.o test_L_infty_DT.o test_L_infty_CP.o
	ar rscv test_lib.a CS_valid_check.o test_csdiv_pts.o test_L_infty_DT.o test_L_infty_CP.o

clean :
	del *.o *.a