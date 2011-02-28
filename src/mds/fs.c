#include "fs.h"
int fs_stat(int ino, struct file_stat * st){
    switch (ino) {
        case 1:
            st -> inode = ino;
            st -> size = 2;
            break;

        case 2:
            st -> inode = ino;
            st -> size = 10;
            break;

        default:
            return -1;
    }
    return 0;
}
