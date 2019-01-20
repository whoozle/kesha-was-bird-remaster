CFLAGS=-mz80
.PHONY=clean all
BUILD_DIR=build

all:
	-mkdir $(BUILD_DIR) 2>/dev/null
	make -C $(BUILD_DIR) -f ../Makefile.build $@

clean:
	rm -rf $(BUILD_DIR)
