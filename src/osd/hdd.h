#include <stdint.h>

#define MAX_HDD_PER_MACHINE 100

typedef struct hdd_space{
    char * path;
    uint64_t totalspace;
    uint64_t availspace;
    uint64_t leavefree;
    /*uint64_t chunkcount;*/
}hdd_space;

//void hdd_get_space(hdd_space * space);
void hdd_refresh_usage(hdd_space *hdd);

void hdd_init(char * config_file);


void calc_store_path(hdd_space * hdd, uint64_t chunkid, char * path);

int select_hdd(int chunksize);
