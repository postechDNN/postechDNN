MAKE = make
DEL = make clean
SUB_DIRS = $(ROOT_SRC_DIR)\DNN_eps $(ROOT_SRC_DIR)\DNN_pddnn

subdirs:
	@for dir in $(SUB_DIRS);do\
		$(MAKE) -C $$dir all;\
		if [ $$? != 0 ]; then exit 1; fi; \
	done

clean :
	@for dir in $(SUB_DIRS);do\
		$(DEL) -C $$dir all;\
		if [ $$? != 0 ]; then exit 1; fi; \
	done