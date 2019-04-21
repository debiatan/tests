#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#define Byte uint8_t
#define U32 uint32_t
#define B32 uint32_t
#define KB(v) 1024L * v
#define MB(v) 1024L * KB(v)

#define internal static
#define ARRAY_LEN(x) ((int)(sizeof(x)/sizeof(*(x))))

typedef struct FieldDescription_t FieldDescription;
struct FieldDescription_t{
    char *s;
    size_t *dest;
};
internal void parse_proc_file(char *fname, FieldDescription *fields, U32 field_count){
    FILE *f = fopen(fname, "rb");

    assert(f);

    char lhs[512];
    char rhs[512];
    do{
        char *dst = lhs;
        while((*dst++ = fgetc(f)) != ':');
        *--dst = 0;

        size_t *dest = 0;

        for(U32 i_field = 0; i_field < field_count; ++i_field){
            if(!strcmp(lhs, fields[i_field].s)){
                dest = fields[i_field].dest;
            }

        }

        if(dest){
            char unit[512];
            fscanf(f, "%lu %s", dest, unit);

            switch(unit[0]){
              case 'k':
                *dest *= 1024; break;
              default: assert(0); break;
            }
        }
        else{
            dst = rhs;
            while((*dst++ = fgetc(f)) != '\n');
            *--dst = 0;
        }

        char c = fgetc(f);
        if(c) ungetc(c, f);
    }while(!feof(f));

    fclose(f);
}

typedef struct MemInfo_t MemInfo;
struct MemInfo_t{
    size_t rss;
    size_t swap;
    size_t committed;
};
internal MemInfo get_mem_info(){
    MemInfo result = {0};

    FieldDescription status_fields[] = { {"VmRSS",  &result.rss}, {"VmSwap", &result.swap} };
    parse_proc_file("/proc/self/status", status_fields, ARRAY_LEN(status_fields));

    FieldDescription meminfo_fields[] = { {"Committed_AS", &result.committed} };
    parse_proc_file("/proc/meminfo", meminfo_fields, ARRAY_LEN(meminfo_fields));

    return result;
}

