#include <stdint.h>

typedef struct hdd_space{
    uint64_t totalspace;
    uint64_t usedspace;
    uint64_t chunkcount;
}hdd_space;

void hdd_get_space(hdd_space * space);


