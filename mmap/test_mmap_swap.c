#include "common.h"

int main(void){
    MemInfo old_mem_info;
    MemInfo mem_info = get_mem_info();
    printf("rss %lu swap %lu\n", mem_info.rss, mem_info.swap);

    size_t total_mmapped = 0;
    size_t allocation_block_size = MB(128);

    while(!mem_info.swap){
        old_mem_info = mem_info;
        Byte *mem = (Byte *) mmap(0, allocation_block_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
        if(mem == MAP_FAILED){
            break;
        }
        else{
            total_mmapped += allocation_block_size;
#if USE_ALLOCATED_MEMORY
            memset(mem, '-', allocation_block_size);
#endif

            mem_info = get_mem_info();
            printf("mmapped %lu rss %lu swap %lu committed (system) %lu\n", 
                   total_mmapped, mem_info.rss, mem_info.swap, mem_info.committed);
        }
    }

    printf("RSS before swapping: %.2f GiB\n", (float)old_mem_info.rss / 1024 / 1024 / 1024);
    printf("Committed (system) before swapping: %.2f GiB\n", (float)old_mem_info.committed / 1024 / 1024 / 1024);

    return 0;
}
