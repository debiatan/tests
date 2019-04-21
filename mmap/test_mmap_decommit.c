#include "common.h"

int main(void){
    size_t allocation_block_size = MB(128);
#define SUMMARY(desc) printf("RSS "desc": %.2f MiB\n", (float)get_mem_info().rss / 1024 / 1024)

    SUMMARY("before allocation");

    Byte *mem = (Byte *) mmap(0, allocation_block_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    memset(mem, '-', allocation_block_size);

    SUMMARY("after allocation and use");

    madvise(mem, allocation_block_size, MADV_DONTNEED);

    SUMMARY("after madvise(DONTNEED)");

    memset(mem, '-', allocation_block_size);

    SUMMARY("after reuse");

    return 0;
}
