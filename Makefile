# -----------------------------------------------------------------------------
# CMake project wrapper Makefile ----------------------------------------------
# -----------------------------------------------------------------------------

SHELL := /bin/bash
RM    := rm -rf
MKDIR := mkdir -p
TEST  := hello

.PHONY: clean run

all: ./build/Makefile
	@ $(MAKE) -C build

./build/Makefile:
	@  ($(MKDIR) build > /dev/null)
	@  (cd build > /dev/null 2>&1 && cmake ..)

clean:
	@  ($(MKDIR) build > /dev/null)
	@  (cd build > /dev/null 2>&1 && cmake .. > /dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.txt

run:
	./server.sh

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
	$(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)
endif
