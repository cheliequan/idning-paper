#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/statvfs.h>
#include <sys/stat.h> 
#include "hdd.h"
#include "random.h"
#include "log.h"


#define HASHSIZE 32768


static int test();
static void hdd_init_dirs(hdd_space * hdd);
hdd_space * hdd_roots[MAX_HDD_PER_MACHINE];
int hdd_root_cnt = 0;


typedef struct hdd_chunk{
    char * path;// the path this chunk store at 
    uint64_t chunkid;
    size_t size;
    struct hdd_chunk * next;
}hdd_chunk;

static hdd_chunk* chunk_hashtable[HASHSIZE];

void chunk_hashtable_put(uint64_t chunkid, hdd_chunk * chunk){

}
void chunk_hashtable_get(uint64_t chunkid, hdd_chunk * chunk){

}

void hdd_scan_chunk(hdd_space *hdd){
    DIR *dd;
    struct dirent *de;

    char fullpath[1024];
    int i, plen;
    strcpy(fullpath, hdd->path);
    strcat(fullpath, "/00/");
    plen = strlen(fullpath);

    for (i=0 ; i<256 ; i++) {
        fullpath[plen-3]="0123456789ABCDEF"[i>>4];
        fullpath[plen-2]="0123456789ABCDEF"[i&15];
        fullpath[plen]='\0';
//      mkdir(fullpath,0755);
        dd = opendir(fullpath);
        if (dd==NULL) {
            continue;
        }
        while ((de = readdir(dd)) != NULL) {
            memcpy(fullpath+plen,de->d_name,17);

            /*hdd_add_chunk(f,fullpath,namechunkid,nameversion);*/
        }
        closedir(dd);
    }



}
// no locks - locked by caller
inline void hdd_refresh_usage(hdd_space *hdd) {
    struct statvfs fsinfo;

    if (statvfs(hdd->path,&fsinfo)<0) {
        hdd->availspace = 0ULL;
        hdd->totalspace = 0ULL;
    }

    //printf("root : %s %d \n", hdd->path, strlen(hdd->path));
    //printf("fsinfo.f_frsize : %ld\n", fsinfo.f_frsize);
    //printf("fsinfo.f_bavail: %ld\n", fsinfo.f_bavail);
    //printf("fsinfo.f_blocks: %ld\n", fsinfo.f_blocks);
    hdd->availspace = (uint64_t)(fsinfo.f_frsize)*(uint64_t)(fsinfo.f_bavail);
    hdd->totalspace = (uint64_t)(fsinfo.f_frsize)*(uint64_t)(fsinfo.f_blocks);
    if (hdd->availspace < hdd->leavefree) {
        hdd->availspace = 0ULL;
    } else {
        hdd->availspace-= hdd->leavefree;
    }
}

//read config file etc/hdd.conf to config hdd roots , the result is in hdd_roots.
void hdd_init(char * config_file){
    FILE * fd;
    char linebuff[1000];
    fd = fopen(config_file, "r");
    if (fd == NULL){
        return ;
    }
    while (fgets(linebuff, 999, fd) != NULL){
        if (linebuff[0] == '#')
            continue;
        linebuff[999] = 0;
        char * start = linebuff;
        while(isspace(*start))
            start++;
        char * end = linebuff + strlen(linebuff)-1;
        while(isspace(*end) || *end == '/'){ //the format should be /mnt/hd1, not /mnt/hd1/
            *end = '\0';
            end --;
        }
        hdd_space * hdd = (hdd_space*)malloc(sizeof(hdd_space));
        hdd->leavefree = 1024;
        hdd->path = strdup(start);
        hdd_refresh_usage(hdd);

        printf("availspace : %"PRIu64"\n", hdd->availspace);
        printf("totalspace : %"PRIu64"\n", hdd->totalspace);
        printf("leavefree : %"PRIu64"\n", hdd->leavefree);

        hdd_roots [hdd_root_cnt++] = hdd;
        hdd_init_dirs(hdd);
    }
    fclose(fd);
    test();
}

static void hdd_init_dirs(hdd_space * hdd){
    char fullpath[1024];
    int i, plen;
    strcpy(fullpath, hdd->path);
    strcat(fullpath, "/00/");


    struct stat sts;
    if ((stat (fullpath, &sts)) != -1)
    { //already exists
        return;
    }

    plen = strlen(fullpath);

    for (i=0 ; i<256 ; i++) {
        fullpath[plen-3]="0123456789ABCDEF"[i>>4];
        fullpath[plen-2]="0123456789ABCDEF"[i&15];
        mkdir(fullpath,0755);
    }
}

void calc_store_path(hdd_space * hdd, uint64_t chunkid, char * path){
    printf("%s", hdd->path);
    printf("%s/%02"PRIX64"/%016"PRIX64, hdd->path, chunkid >> 48, chunkid);
    sprintf(path, "%s/%02"PRIX64"/%016"PRIX64, hdd->path, chunkid >> 48, chunkid);
}

/*
 * 输入chunksize,
 * 输出为一个文件保存路径.
 * hdd_roots 按从大到小顺序排序，选择的时候，排在前面的hdd选择可能性大.
 * */
int select_hdd(){
    static int cnt = 0;
    if (cnt % 100 == 0){
        /*qsort();// */
        //TODO
    }
    int id = rand_exp(hdd_root_cnt);
    return id;
}


static int test(){
    char path[1024];
    calc_store_path(hdd_roots[0], 0xfff, path);
    logging(LOG_DEUBG, "%s", path);
    return 0;
}

