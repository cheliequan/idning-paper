#define FUSE_USE_VERSION 26

#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <http_client.h>
#include "mds_conn.h"
#include "log.h"


static const char *hello_name = "hello";

static void test(){
    fuse_ino_t arr[1] ;
    struct file_stat stat_arr[1];
    arr[0] = 2;
    stat_send_request(arr, 1, stat_arr);
}

static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
    DBG();
    fprintf(stderr, "hello_stat(%d)\n", (int)ino);
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
        stbuf->st_mode = S_IFREG | 0777;
        stbuf->st_nlink = 1;
    }

	return 0;
}

static void hello_ll_getattr(fuse_req_t req, fuse_ino_t ino,
			     struct fuse_file_info *fi)
{
    DBG();
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

static void hello_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    DBG();
    fprintf(stderr, "hello_ll_lookup: %ld, %s", parent, name);
	struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));

    struct file_stat * stat = file_stat_new();
    lookup_send_request(parent, name, stat);
    e.ino = stat -> ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;
    fprintf(stderr, "--------------------- hello_ll_lookup find inode : %ld \n", e.ino);

    if (e.ino){ // exists
        hello_stat(e.ino, &e.attr);
    }

    /*if (0 == strcmp(name , "hello_")){*/
        /*e.ino = 2;*/
        /*e.attr_timeout = 1.0;*/
        /*e.entry_timeout = 1.0;*/
        /*hello_stat(e.ino, &e.attr);*/
    /*} else {*/
        /*e.ino = 0;*/
        /*e.attr_timeout = 1.0;*/
    /*}*/


    fuse_reply_entry(req, &e);
}

struct dirbuf {
	char *p;
	size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
		       fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
			  b->size);
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
	(void) fi;

    struct file_stat stat_arr[100]; //TODO: 100...
    int cnt = ls_send_request(ino, &stat_arr);
    int i;
    struct dirbuf b;
    memset(&b, 0, sizeof(b));
    dirbuf_add(req, &b, ".", 1);
    dirbuf_add(req, &b, "..", 1);
    /*dirbuf_add(req, &b, "xxxxxx", 3);*/
    for(i=0; i<cnt; i++){
        fprintf(stderr, "stat[%d].name: %s", i, stat_arr[i].name);
        fprintf(stderr, "stat[%d].ino : %d", i, stat_arr[i].ino);
        dirbuf_add(req, &b, stat_arr[i].name, stat_arr[i].ino);
    }

    reply_buf_limited(req, b.p, b.size, off, size);
    free(b.p);
}

static void hello_ll_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi)
{
    DBG();
    fprintf(stderr, "hello_ll_open : ino:  %ld\n", ino);

	/*if ((fi->flags & 3) != O_RDONLY)*/
		/*fuse_reply_err(req, EACCES);*/
	/*else*/
		/*fuse_reply_open(req, fi);*/

    fi->fh = ino; //TODO: this is tmp
    /*fi->direct_io = 1;*/
    /*fi->keep_cache=1;*/


    fuse_reply_open(req, fi);
}

static void hello_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi)
{
    DBG();
	(void) fi;

	assert(ino == 2);
    /*char url[256] = "http://10.100.1.76/";*/
    /*sprintf(url, "http://10.100.1.76:6006/get/%d", ino);*/

    char url[256] = "http://127.0.0.1/";
    sprintf(url, "http://127.0.0.1:6006/get/%ld", ino);
    fprintf(stderr, "http_get: %s \n", url);
    //
    http_response * response = http_get(url);
    if (response){
        int len = evbuffer_get_length(response->body);
        uint8_t * buf = alloca(len);
        evbuffer_copyout(response->body, buf, len);

        fuse_reply_buf(req, buf+off, size);
    }else{
        //TODO: this is  no replay ,will hold
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
void my_ll_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *in_stbuf, int to_set, struct fuse_file_info *fi) {
    DBG();
    struct stat stbuf;
    uint64_t maxfleng;
    uint8_t attr[35];
    int status;
    const struct fuse_ctx *ctx;
    uint8_t setmask = 0;


	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);

    /*fuse_reply_attr(req, &o_stbuf, (mfs_attr_get_mattr(attr)&MATTR_NOACACHE)?0.0:attr_cache_timeout);*/
}


void my_ll_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
    DBG();
    int err = 0;
    /*if (off>=MAX_FILE_SIZE || off+size>=MAX_FILE_SIZE) {*/
        /*fuse_reply_err(req, EFBIG);*/
        /*return;*/
    /*}*/
    /*err = write_data(fileinfo->data,off,size,(const uint8_t*)buf);*/
    struct evbuffer *  evb = evbuffer_new();
    evbuffer_add(evb, buf+off, size);

    /*TODO : evbuffer_add_reference();*/

    char url[256];
    sprintf(url, "http://127.0.0.1:6006/put/%ld", ino);

    http_response * response = http_post(url, evb);
    int len = evbuffer_get_length(response->body);

    if (err!=0) {
        fuse_reply_err(req,err);
    } else {
        fuse_reply_write(req,size);
    }
}


void my_ll_flush(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi) {
    int err = 0;
    fuse_reply_err(req,err);
}


   
void my_ll_create(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi) {
    struct fuse_entry_param e;
    uint32_t inode;
    uint8_t oflags;
    uint8_t attr[35];
    uint8_t mattr;
    uint32_t nleng;
    int status;
    const struct fuse_ctx *ctx;
    /*finfo *fileinfo;*/

    fprintf(stderr,"create (%lu,%s,%04o)\n",(unsigned long int)parent,name,mode);
    struct file_stat * stat = file_stat_new();
    mknod_send_request(parent, name, S_IFREG, mode, stat);

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;

    e.attr_timeout = 0.0;
    e.entry_timeout = 0.0;
    /*mfs_attr_to_stat(inode,attr,&e.attr);*/
    if (fuse_reply_create(req, &e, fi) == -ENOENT) {

    }

    /*oflags = AFTER_CREATE;*/
    /*if ((fi->flags & O_ACCMODE) == O_RDONLY) {*/
        /*oflags |= WANT_READ;*/
    /*} else if ((fi->flags & O_ACCMODE) == O_WRONLY) {*/
        /*oflags |= WANT_WRITE;*/
    /*} else if ((fi->flags & O_ACCMODE) == O_RDWR) {*/
        /*oflags |= WANT_READ | WANT_WRITE;*/
    /*} else {*/
        /*fuse_reply_err(req, EINVAL);*/
    /*}*/

    /*ctx = fuse_req_ctx(req);*/
    /*status = fs_mknod(parent,nleng,(const uint8_t*)name,TYPE_FILE,mode&07777,ctx->uid,ctx->gid,0,&inode,attr);*/
    /*status = mfs_errorconv(status);*/
    /*if (status!=0) {*/
        /*fuse_reply_err(req, status);*/
        /*return;*/
    /*}*/
    /*status = fs_opencheck(inode,ctx->uid,ctx->gid,oflags,NULL);*/
    /*status = mfs_errorconv(status);*/
    /*if (status!=0) {*/
        /*fuse_reply_err(req, status);*/
        /*return;*/
    /*}*/

    /*mattr = mfs_attr_get_mattr(attr);*/
    /*fileinfo = mfs_newfileinfo(fi->flags & O_ACCMODE,inode);*/
    /*fi->fh = (unsigned long)fileinfo;*/
    /*if (keep_cache==1) {*/
        /*fi->keep_cache=1;*/
    /*} else if (keep_cache==2) {*/
        /*fi->keep_cache=0;*/
    /*} else {*/
        /*fi->keep_cache = (mattr&MATTR_ALLOWDATACACHE)?1:0;*/
    /*}*/
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
};

int main(int argc, char *argv[])
{
    mds_conn_init();
    ping_send_request();
    test();

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

