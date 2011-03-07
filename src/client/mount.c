#define FUSE_USE_VERSION 26

#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/queue.h>

#include "http_client.h"
#include "mds_conn.h"
#include "log.h"
#include "protocol.gen.h"


static const char *hello_name = "hello";


static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
    DBG();
    
    logging(LOG_DEUBG, "stat(%lu)", ino);
    fuse_ino_t arr[1] ;
    struct file_stat stat_arr[1];
    arr[0] = ino;

    stbuf->st_ino = ino;
    stbuf->st_uid = 0;
    stbuf->st_gid = 0;
    if (ino == 1){
        stbuf->st_mode = S_IFDIR | 0777;
        stbuf->st_nlink = 2;
    }else{
        stat_send_request(arr, 1, stat_arr);
        stbuf->st_size = stat_arr[0].size;
        stbuf->st_mode = stat_arr[0].mode | 0777;
        /*stbuf->st_mode = S_IFDIR| 0777; //TODO*/
        logging(LOG_DEUBG, "stat (ino = %lu) return {size: %d, mode: %04o}", 
                ino, stbuf->st_size, stbuf->st_mode);
        stbuf->st_nlink = 1;
    }

	return 0;
}

static void hello_ll_getattr(fuse_req_t req, fuse_ino_t ino,
			     struct fuse_file_info *fi)
{
    DBG();
    logging(LOG_DEUBG, "getattr(%lu)", ino);
	struct stat stbuf;
	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

static void hello_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    DBG();
    logging(LOG_DEUBG, "lookup(parent = %lu, name = %s)", parent, name);
	struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));

    struct file_stat * stat = file_stat_new();
    lookup_send_request(parent, name, stat);
    e.ino = stat -> ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;

    logging(LOG_DEUBG, "lookup(parent = %lu, name = %s) return inode: %lu", parent, name, e.ino);

    if (e.ino){ // exists
        hello_stat(e.ino, &e.attr);
    }
    fuse_reply_entry(req, &e);
}

struct dirbuf {
	char *p;
	size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name, int mode,
		       fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
    /*stbuf.st_mode = mode;*/

    stbuf.st_mode = S_IFREG;
    printf("S_IFREG: %04o\n", S_IFREG);
    printf("S_IFDIR: %04o\n", S_IFDIR);
    /*stbuf.st_mode = */

	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf, b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
			     off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, NULL, 0);
}

static void hello_ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi)
{
    DBG();
    logging(LOG_DEUBG, "readdir(ino = %lu)", ino);
	(void) fi;

    struct file_stat stat_arr[100]; //TODO: 100...
    int cnt = ls_send_request(ino, &stat_arr);
    int i;
    struct dirbuf b;
    memset(&b, 0, sizeof(b));
    /*dirbuf_add(req, &b, ".",  S_IFDIR, 1);*/
    /*dirbuf_add(req, &b, "..", S_IFDIR, 1);*/
    /*dirbuf_add(req, &b, "xxxxxx", 3);*/
    for(i=0; i<cnt; i++){
        logging(LOG_DEUBG, "readdir(parent = %lu) return {ino: %lu, name : %s, mode: %04o}", 
                ino, stat_arr[i].ino, stat_arr[i].name, stat_arr[i].mode);
        dirbuf_add(req, &b, stat_arr[i].name, stat_arr[i].mode, stat_arr[i].ino);
    }

    reply_buf_limited(req, b.p, b.size, off, size);
    free(b.p);
}

static void hello_ll_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi)
{
    DBG();
    logging(LOG_DEUBG, "open(%lu)", ino);

	/*if ((fi->flags & 3) != O_RDONLY)*/
		/*fuse_reply_err(req, EACCES);*/
	/*else*/
		/*fuse_reply_open(req, fi);*/

    /*fi->fh = ino; //TODO: this is tmp*/
    /*fi->direct_io = 1;*/
    /*fi->keep_cache=1;*/


    fuse_reply_open(req, fi);
}

static void hello_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi)
{
    DBG();
    logging(LOG_DEUBG, "read (%lu, size=%d, off=%d)", ino, size, off);
	(void) fi;

    /*char url[256] = "http://10.100.1.76/";*/
    /*sprintf(url, "http://10.100.1.76:6006/get/%d", ino);*/

    char url[256] = "http://127.0.0.1/";
    sprintf(url, "http://127.0.0.1:6006/get/%lu", ino);
    fprintf(stderr, "http_get: %s \n", url);
    //
    http_response * response = http_get(url, NULL);
    if (response){
        int len = evbuffer_get_length(response->body);
        uint8_t * buf = alloca(len);
        evbuffer_copyout(response->body, buf, len);

        fuse_reply_buf(req, buf+off, size);
    }else{
        //TODO: this is  no replay ,will hold
    }
}



void my_ll_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
    DBG();
    logging(LOG_DEUBG, "write(%lu, size=%d, off=%d)", ino, size, off);
    int err = 0;
    /*if (off>=MAX_FILE_SIZE || off+size>=MAX_FILE_SIZE) {*/
        /*fuse_reply_err(req, EFBIG);*/
        /*return;*/
    /*}*/
    /*err = write_data(fileinfo->data,off,size,(const uint8_t*)buf);*/
    struct evbuffer *  evb = evbuffer_new();
    evbuffer_add(evb, buf, size);

    /*TODO : evbuffer_add_reference();*/

    char url[256];
    sprintf(url, "http://127.0.0.1:6006/put/%lu", ino);


    struct evkeyvalq * headers = (struct evkeyvalq *) malloc( sizeof(struct evkeyvalq));
    TAILQ_INIT(headers);
    char range[255];
    sprintf(range, "bytes=%d-%d", off, off+size-1);
    logging(LOG_DEUBG, range);

    evhttp_add_header(headers, "Range", range);

    http_response * response = http_post(url, headers, evb);
    int len = evbuffer_get_length(response->body);
    free(headers);

	/*struct stat stbuf;*/
	/*memset(&stbuf, 0, sizeof(stbuf));*/
	/*if (hello_stat(ino, &stbuf) == -1)*/
		/*fuse_reply_err(req, ENOENT);*/
    /*stbuf.size = 8;*/


    //do stat
	struct stat stbuf;
	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);


    if (stbuf.st_size < off+size){
        //修改文件size
        struct file_stat * f_stat = file_stat_new();
        f_stat -> ino = ino;
        f_stat -> size = off+size;

        setattr_send_request(f_stat);
    }

    if (err!=0) {
        fuse_reply_err(req,err);
    } else {
        fuse_reply_write(req,size);
    }
}

/* 
实现了write 函数后.
不能write, 
报如下错：
unique: 21, opcode: SETATTR (4), nodeid: 2, insize: 128
   unique: 21, error: -38 (Function not implemented), outsize: 16
unique: 22, opcode: FORGET (2), nodeid: 2, insize: 48

网上有人说：
The attributes of a filesystem element are set by the filesystem itself. You can only define any attributes of a filesystem element when you open() it. For example, a file can be opened read-only. Once it is open, this status can only be gotten, and not set. The exceptions would be the chown()/chmod() operations, which have their own specific handlers.
Fuse is a very cool thing.
--- rod.

*/
/*
*/

/*
 * 包括st_atime
 * 包括st_mtime
 * 包括st_ctime
 *
 * st_size
 * st_uid
 * st_gid
 * st_mode
 * st_nlink
 *
 * 采用方法如戏下：
 * 先取的stat, 在client端进行set, 然后再写回mds, 
 * mfs不一样，mfs是直接向mds发送set命令.
 *
 *
 * */
void my_ll_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *in_stbuf, int to_set, struct fuse_file_info *fi) {
    DBG();
    logging(LOG_DEUBG, "setattr(%lu)", ino);
    struct file_stat * fstat = file_stat_new();

    fuse_ino_t arr[1] ;
    arr[0] = ino;

    stat_send_request(arr, 1, fstat);

    if (to_set & FUSE_SET_ATTR_MODE) {
        fstat ->mode = in_stbuf->st_mode| 0777;
        logging(LOG_DEUBG, "setattr(%lu) set mode = %04o", ino, in_stbuf->st_mode);
    }
    if (to_set & FUSE_SET_ATTR_SIZE) {
        fstat->size = in_stbuf->st_size;
        logging(LOG_DEUBG, "setattr(%lu) set size = %d", ino, in_stbuf->st_size);
    }
    setattr_send_request(fstat);

	struct stat stbuf;
	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
    fuse_reply_attr(req, &stbuf, 1.0);
}

void my_ll_flush(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi) {
    int err = 0;
    fuse_reply_err(req,err);
}


void my_ll_create(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi) {
    logging(LOG_DEUBG, "create(parent = %lu, name = %s, mode=%04o)", parent, name, mode);
    struct fuse_entry_param e;
    struct file_stat * stat = file_stat_new();
    mknod_send_request(parent, name, 0, S_IFREG, stat);

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 0.0;
    e.entry_timeout = 0.0;
    hello_stat(e.ino, &e.attr);

    if (fuse_reply_create(req, &e, fi) == -ENOENT) {

    }
}

void my_ll_mkdir(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode) {
    logging(LOG_DEUBG, "mkdir(parent = %lu, name = %s, mode=%04o)", parent, name, mode);
    struct fuse_entry_param e;
    struct file_stat * stat = file_stat_new();
    mknod_send_request(parent, name, 0, S_IFDIR, stat);

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 0.0;
    e.entry_timeout = 0.0;

    hello_stat(e.ino, &e.attr);

    if (fuse_reply_entry(req, &e) == -ENOENT) {

    }
}
 


static void my_ll_unlink(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    DBG();
    logging(LOG_DEUBG, "unlink(parent = %lu, name = %s)", parent, name);

    unlink_send_request(parent, name);

    fuse_reply_err(req, 0);
}


static struct fuse_lowlevel_ops hello_ll_oper = {
	.lookup		= hello_ll_lookup,
	.getattr	= hello_ll_getattr,
	.readdir	= hello_ll_readdir,
	.open		= hello_ll_open,
	.read		= hello_ll_read,
	.write      = my_ll_write,
	.setattr    = my_ll_setattr,
	.flush      = my_ll_flush,
	.create     = my_ll_create,
	.mkdir      = my_ll_mkdir,
	.unlink     = my_ll_unlink,
	.rmdir      = my_ll_unlink,
};

int main(int argc, char *argv[])
{
    mds_conn_init();
    ping_send_request();

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan *ch;
	char *mountpoint;
	int err = -1;

	if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
	    (ch = fuse_mount(mountpoint, &args)) != NULL) {
		struct fuse_session *se;

		se = fuse_lowlevel_new(&args, &hello_ll_oper,
				       sizeof(hello_ll_oper), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint, ch);
	}
	fuse_opt_free_args(&args);

	return err ? 1 : 0;
}

