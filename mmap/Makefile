CC = clang
ACCEPTED_WARNINGS = -Wno-unused-function -Wno-unused-variable -Wno-missing-field-initializers
WARNING_FLAGS = -Wall -Wextra -Wpedantic -Werror $(ACCEPTED_WARNINGS)
CCFLAGS = -O0 -g $(WARNING_FLAGS) -fno-strict-aliasing 

all: test_mmap_swap test_mmap_swap_unused_memory test_mmap_decommit

test_mmap_swap: test_mmap_swap.c
	$(CC) $(CCFLAGS) -DUSE_ALLOCATED_MEMORY $< -o $@ 

test_mmap_swap_unused_memory: test_mmap_swap.c
	$(CC) $(CCFLAGS) $< -o $@ 

test_mmap_decommit: test_mmap_decommit.c
	$(CC) $(CCFLAGS) $< -o $@ 

.PHONY: clean 
clean:
	$(RM) test_mmap_decommit test_mmap_swap test_mmap_swap_unused_memory
