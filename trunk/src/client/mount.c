#define FUSE_USE_VERSION 26

#include "sfs_common.h"
#include <fuse_lowlevel.h>
#include <pthread.h>

#include "cmgr_conn.h"
#include "osd_conn.h"
#include "mds_conn.h"
#include "attr_cache.h"

static void fill_stbuf(struct stat* stbuf, struct file_stat * stat);


static int get_mid_of_ino(int ino)
{
    struct file_stat *cached = attr_cache_lookup(ino);
    if (cached == NULL) {
        /*cached = find_in_all(); */
        assert(0);
    }
    if (cached == NULL) {
        // return err
        assert(0);

    }
    int mid = cached->pos_arr[0];
    return mid;
}

static inline struct machine *get_machine_of_parent_inode(fuse_ino_t ino)
{
    struct file_stat *cached = attr_cache_lookup(ino);
    int mid = get_mid_of_ino(cached->parent_ino);
    struct machine *m = cluster_get_machine_by_mid(mid);
    return m;
}

static inline struct machine *get_machine_of_inode(fuse_ino_t ino)
{
    int mid = get_mid_of_ino(ino);
    struct machine *m = cluster_get_machine_by_mid(mid);
    return m;
}



struct dirbuf {
    char *p;
    size_t size;
};
//TODO: 优化.
static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
                       struct file_stat * stat)
{
    struct stat stbuf;
    size_t oldsize = b->size;
    b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
    b->p = (char *)realloc(b->p, b->size);
    memset(&stbuf, 0, sizeof(stbuf));
    fill_stbuf(&stbuf, stat);
    fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
                      b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
                             off_t off, size_t maxsize)
{
    if (off < bufsize)
        return fuse_reply_buf(req, buf + off, min(bufsize - off, maxsize));
    else
        return fuse_reply_buf(req, NULL, 0);
}

/*
 * get file_stat form cache  
 * if the cache is out of date
 * get it from mds
 * the returned file_stat is Already in the cache , so we do not need to free it!
 * */
static struct file_stat *get_attr(fuse_ino_t ino)
{
    int mid;
    logging(LOG_DEUBG, "ning_get_attr %d", ino);
    struct file_stat *cached = attr_cache_lookup(ino);
    /*if (S_ISDIR(cached->mode)) {*/
        /*mid = get_mid_of_ino(ino);*/
    /*} else {*/
        mid = get_mid_of_ino(cached->parent_ino);
    /*}*/
    //?? out of date??
    /*struct machine *m = cluster_get_machine_by_mid(mid);*/
    /*uint64_t arr[1];*/
    /*arr[0] = ino;*/

    //log_file_stat("stat before updat in cache:", cached);
    //stat_send_request(m->ip, m->port, arr, 1, cached);  // this will update the cache item
    //log_file_stat("stat updated in cache:", cached);
    return cached;
}

static void fill_stbuf(struct stat* stbuf, struct file_stat * stat){
    stbuf->st_ino = stat->ino;
    stbuf->st_uid = 0;
    stbuf->st_gid = 0;
    stbuf->st_size = stat->size;
    stbuf->st_blksize = 1024 * 1024 * 1024;
    stbuf->st_mode = stat->mode | 0777;
    if (S_ISDIR(stat->mode)) {
        stbuf->st_nlink = 2;
    }else{
        stbuf->st_nlink = 1;
    }
}

static int sfs_stat(fuse_ino_t ino, struct stat *stbuf)
{
    logging(LOG_DEUBG, "stat(%lu)", ino);
    struct file_stat *stat = get_attr(ino);
    fill_stbuf(stbuf, stat);

    return 0;
}

static void sfs_ll_getattr(fuse_req_t req, fuse_ino_t ino,
                           struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "getattr(%lu)", ino);
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    if (sfs_stat((uint64_t) ino, &stbuf) == -1)
        fuse_reply_err(req, ENOENT);
    else
        fuse_reply_attr(req, &stbuf, 1.0);
}

static void sfs_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    logging(LOG_DEUBG, "lookup(parent = %lu, name = %s)", parent, name);
    struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));

    struct file_stat *stat = file_stat_new();
    struct machine *m = get_machine_of_inode(parent);
    lookup_send_request(m->ip, m->port, (uint64_t) parent, name, stat);
    logging(LOG_DEUBG, "lookup(parent = %lu, name = %s) return inode: %lu",
            parent, name, e.ino);
    if (stat->ino) {
        attr_cache_add(stat);
        e.ino = stat->ino;
        e.attr_timeout = 1.0;
        e.entry_timeout = 1.0;
        sfs_stat(e.ino, &e.attr);   // fill e.attr , should not call rpc again
    } else {
        e.ino = 0;
        file_stat_free(stat);
    }
    fuse_reply_entry(req, &e);
}


static void sfs_ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
                           off_t off, struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "readdir(ino = %lu)", ino);
    (void)fi;

    struct file_stat **stat_arr;
    int cnt;

    struct machine *m = get_machine_of_inode(ino);
    ls_send_request(m->ip, m->port, ino, &stat_arr, &cnt);
    int i;
    struct dirbuf b;
    memset(&b, 0, sizeof(b));
    for (i = 0; i < cnt; i++) {
        log_file_stat("readdir return :", stat_arr[i]);

        if ((strcmp(stat_arr[i]->name, ".") != 0) && (strcmp(stat_arr[i]->name, "..") != 0))
            attr_cache_add(stat_arr[i]);

        dirbuf_add(req, &b, stat_arr[i]->name, stat_arr[i]);
    }

    reply_buf_limited(req, b.p, b.size, off, size);
    free(b.p);
    if (stat_arr)
        free(stat_arr);
}

static void sfs_ll_open(fuse_req_t req, fuse_ino_t ino,
                        struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "open(%lu)", ino);

    fi->fh = (unsigned long)get_attr(ino);

    fuse_reply_open(req, fi);
}

void sfs_ll_readlink(fuse_req_t req, fuse_ino_t ino)
{
    logging(LOG_DEUBG, "readlink(ino = %lu)", ino);
    struct machine *m = get_machine_of_parent_inode(ino);
    const char *path = readlink_send_request(m->ip, m->port, ino);
    logging(LOG_DEUBG, "sfs_ll_readlink get path: %s", path);

    if (0) {
        fuse_reply_err(req, 3);
    } else {
        fuse_reply_readlink(req, (char *)path);
    }

    free((void *)path);
}

void sfs_ll_statfs(fuse_req_t req, fuse_ino_t ino)
{
    uint32_t totalspace, availspace;
    uint32_t inodes;

    uint32_t bsize = 1024;
    struct statvfs stfsbuf;
    memset(&stfsbuf, 0, sizeof(stfsbuf));

    (void)ino;
    int *mds;
    int mds_cnt;
    cluster_get_mds_arr(&mds, &mds_cnt);
    struct machine *m = cluster_get_machine_by_mid(mds[0]); // TODO: currently it's  a random one
    statfs_send_request(m->ip, m->port, &totalspace, &availspace, &inodes);
    logging(LOG_DEUBG, "sfs_ll_statfs get : %d , %d, %d ", totalspace, availspace,
            inodes);

    stfsbuf.f_namemax = 1024 * 1024;
    stfsbuf.f_frsize = bsize;
    stfsbuf.f_bsize = bsize;

    stfsbuf.f_blocks = totalspace / bsize;
    stfsbuf.f_bfree = availspace / bsize;
    stfsbuf.f_bavail = availspace / bsize;

    stfsbuf.f_files = inodes;
    stfsbuf.f_ffree = 1000000000;
    stfsbuf.f_favail = 1000000000;
    fuse_reply_statfs(req, &stfsbuf);
}

static void sfs_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
                        off_t off, struct fuse_file_info *fi)
{
    // size -> long unsigned int 
    // off -> int64
    logging(LOG_DEUBG, "read (%lu, size=%ld, off=%" PRIu64 ")", ino, size, off);    //read (3, size=4096, off=0)

    (void)fi;
    //FIXME , 6006

    char url[256] = "http://127.0.0.1/";
    sprintf(url, "http://127.0.0.1:6006/get/%lu", ino);

    struct evkeyvalq *headers =
        (struct evkeyvalq *)malloc(sizeof(struct evkeyvalq));
    TAILQ_INIT(headers);
    char range[255];

    sprintf(range, "bytes=%" PRIu64 "-%" PRIu64 "", off, off + size - 1);
    logging(LOG_DEUBG, "Range: %s", range);

    evhttp_add_header(headers, "Range", range);

    //
    http_response *response = http_get(url, headers);
    if (response) {
        int len = evbuffer_get_length(response->body);
        logging(LOG_DEUBG, "data in response: %d bytes", len);
        /*if (len == 0){*/
            /*fuse_reply_err(req, EINVAL);*/
        
        /*}else{*/
            uint8_t *buf = alloca(len);
            evbuffer_copyout(response->body, buf, len);
            evhttp_clear_headers(headers);
            free(headers);

            fuse_reply_buf(req, buf, len);
        /*}*/
        http_response_free(response);
    } else {
        assert(0);
    }
}

void sfs_ll_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size,
                  off_t off, struct fuse_file_info *fi)
{
    DBG();
    logging(LOG_DEUBG, "write (%lu, size=%ld, off=%" PRIu64 ")", ino, size,
            off);
    int err = 0;

    struct file_stat *stat = (struct file_stat *)(unsigned long)(fi->fh);
    logging(LOG_DEUBG, "ready to write content on pos [%d, %d]",
            stat->pos_arr[0], stat->pos_arr[1]);

    buffered_write(stat, off, (uint64_t) size, buf);

    if (err != 0) {
        fuse_reply_err(req, err);
    } else {
        fuse_reply_write(req, size);
    }
}


void sfs_ll_flush(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "flush(ino= %lu)", ino);

    int err = 0;
    struct file_stat *stat = (struct file_stat *)(unsigned long)(fi->fh);
    int sizenow = buffered_write_flush(stat);

    //do stat
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    if (sfs_stat((uint64_t) ino, &stbuf) == -1)
        fuse_reply_err(req, ENOENT);

    struct file_stat *f_stat = get_attr(ino);
    if (stbuf.st_size < sizenow) {
        //修改文件size
        f_stat->ino = ino;
        f_stat->size = sizenow;



        struct file_stat *cached_parent = attr_cache_lookup(f_stat->parent_ino);
        int i;
        for (i=0;i<2; i++){
            int mid = cached_parent->pos_arr[i];
            struct machine *m = cluster_get_machine_by_mid(mid);
            setattr_send_request(m->ip, m->port, f_stat);
        }


        /*struct machine *m = get_machine_of_parent_inode(f_stat->ino);*/
        /*setattr_send_request(m->ip, m->port, f_stat);*/
    }

    fuse_reply_err(req, err);
}




int create_mds_req_cb(struct mds_req_ctx * ctx, void* ptr){
    DBG();
    if (--ctx->count){
        return 1; 
    }
    struct fuse_entry_param e;
    struct file_stat * stat = (struct file_stat *) ptr;
    log_file_stat("create return :", stat);
    attr_cache_add(stat);       //no free

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;
    sfs_stat(e.ino, &e.attr);

    //get attr
    ctx->fi->fh = (unsigned long)stat;

    //for test:
    //
    /*struct mds_req_ctx* ctx = mds_req_ctx_new( req, e, fi);*/
    if (fuse_reply_create(ctx->req, &e, ctx->fi) == -ENOENT) {
        assert(0);
    }
    return 0;
}

struct mds_req_ctx* mds_req_ctx_new( fuse_req_t req, struct fuse_file_info * fi){
    struct mds_req_ctx * ctx = malloc(sizeof(struct mds_req_ctx));
    ctx->req = req;
    ctx->fi = fi;
    ctx->count = 2;
    ctx->cb = create_mds_req_cb;
    return ctx;
}


void sfs_ll_create_async(fuse_req_t req, fuse_ino_t parent, const char *name,
                   mode_t mode, struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "create_async(parent = %lu, name = %s, mode=%04o)", parent,
            name, mode);
    uint64_t ino = cmgr_get_uuid();

    struct file_stat *cached_parent = attr_cache_lookup(parent);
    struct mds_req_ctx * ctx = mds_req_ctx_new(req, fi);
    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        mknod_send_request_async(m->ip, m->port, parent, ino, name, 0, S_IFREG, ctx);
    }
    event_dispatch(); //FIXME
}


void sfs_ll_create(fuse_req_t req, fuse_ino_t parent, const char *name,
                   mode_t mode, struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "create(parent = %lu, name = %s, mode=%04o)", parent,
            name, mode);
    struct fuse_entry_param e;
    uint64_t ino = cmgr_get_uuid();

    struct file_stat *cached_parent = attr_cache_lookup(parent);
    struct file_stat *stat = file_stat_new();

    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        mknod_send_request(m->ip, m->port, parent, ino, name, 0, S_IFREG, stat);
    }

    log_file_stat("create return :", stat);
    attr_cache_add(stat);       //no free

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;
    sfs_stat(e.ino, &e.attr);

    //get attr
    fi->fh = (unsigned long)stat;

    if (fuse_reply_create(req, &e, fi) == -ENOENT) {

    }
}


void sfs_ll_mkdir(fuse_req_t req, fuse_ino_t parent, const char *name,
                  mode_t mode)
{
    logging(LOG_DEUBG, "mkdir(parent = %lu, name = %s, mode=%04o)", parent,
            name, mode);
    struct fuse_entry_param e;
    /*struct machine *m = get_machine_of_parent_inode(parent);*/
    struct file_stat *stat = file_stat_new();
    uint64_t ino = cmgr_get_uuid();


    struct file_stat *cached_parent = attr_cache_lookup(parent);

    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        mknod_send_request(m->ip, m->port, parent, ino, name, 0, S_IFDIR, stat);
    }

    log_file_stat("create return :", stat);
    attr_cache_add(stat);       //no free

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;

    sfs_stat(e.ino, &e.attr);

    if (fuse_reply_entry(req, &e) == -ENOENT) {

    }
}

void sfs_ll_symlink(fuse_req_t req, const char *path, fuse_ino_t parent,
                 const char *name)
{
    logging(LOG_DEUBG, "symlink(parent = %lu, name = %s, path=%s)", parent,
            name, path);
    struct fuse_entry_param e;
    uint64_t ino = cmgr_get_uuid();

    struct file_stat *cached_parent = attr_cache_lookup(parent);
    struct file_stat *stat = file_stat_new();

    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        symlink_send_request(m->ip, m->port, parent, ino, name, path, stat);
    }

    log_file_stat("symlink return :", stat);
    attr_cache_add(stat);       //no free

    memset(&e, 0, sizeof(e));
    e.ino = stat->ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;

    sfs_stat(e.ino, &e.attr);

    if (fuse_reply_entry(req, &e) == -ENOENT) {

    }
}

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
void sfs_ll_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *in_stbuf,
                    int to_set, struct fuse_file_info *fi)
{
    logging(LOG_DEUBG, "setattr(%lu)", ino);
    struct file_stat *old_stat = get_attr(ino);

    if (to_set & FUSE_SET_ATTR_MODE) {
        old_stat->mode = in_stbuf->st_mode | 0777;
        logging(LOG_DEUBG, "setattr(%lu) set mode = %04o", ino,
                in_stbuf->st_mode);
    }
    if (to_set & FUSE_SET_ATTR_SIZE) {
        old_stat->size = in_stbuf->st_size;
        logging(LOG_DEUBG, "setattr(%lu) set size = %d", ino,
                in_stbuf->st_size);
    }


    struct file_stat *cached = attr_cache_lookup(ino);
    struct file_stat *cached_parent = attr_cache_lookup(cached->parent_ino);

    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        setattr_send_request(m->ip, m->port, old_stat);
    }

    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    if (sfs_stat(ino, &stbuf) == -1)
        fuse_reply_err(req, ENOENT);
    fuse_reply_attr(req, &stbuf, 1.0);
}


static void sfs_ll_unlink(fuse_req_t req, fuse_ino_t parent, const char *name)
{
    logging(LOG_DEUBG, "unlink(parent = %lu, name = %s)", parent, name);


    struct file_stat *cached_parent = attr_cache_lookup(parent);

    int i;
    for (i=0;i<2; i++){
        int mid = cached_parent->pos_arr[i];
        struct machine *m = cluster_get_machine_by_mid(mid);
        unlink_send_request(m->ip, m->port, parent, name);
    }
    fuse_reply_err(req, 0);
}

void sfs_ll_release(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
    /*struct file_stat *stat = (struct file_stat *)(unsigned long)(fi->fh);*/
    fuse_reply_err(req, 0);
}

void sfs_ll_access(fuse_req_t req, fuse_ino_t ino, int mask)
{
    DBG();
    if (0) {
        assert(0);
    } else {
        fuse_reply_err(req, 0);
    }
}

void sfs_ll_init(int debug_mode_in, int keep_cache_in,
              double direntry_cache_timeout_in, double entry_cache_timeout_in,
              double attr_cache_timeout_in)
{
}


#define RST_FOUND 0

int search_inode_over_all_mds(uint64_t ino)
{
    DBG();

    int *mds;
    int mds_cnt;
    int i;
    int64_t ino_arr[1] ;
    ino_arr[0] = ino;

    cluster_get_mds_arr(&mds, &mds_cnt);
    for (i = 0; i < mds_cnt; i++) {
       struct file_stat *stat = file_stat_new();
        /*EVTAG_ARRAY_ADD_VALUE(stat, pos_arr, mds[i]);*/
        struct machine *m = cluster_get_machine_by_mid(mds[i]);

        if (stat_send_request(m->ip, m->port, ino_arr, 1, stat) == RST_FOUND) {
            logging(LOG_INFO, "get inode %" PRIu64 " at mds (%d)", (uint64_t) ino,
                    mds[i]);
            attr_cache_add(stat);   //no free

            return 1;
        }
        file_stat_free(stat);
    }
    return 0;
}

int find_root(){
    return search_inode_over_all_mds(1);
}
void sfs_mkfs()
{
    DBG();

    int *mds;
    int mds_cnt;
    cluster_get_mds_arr(&mds, &mds_cnt);
    if (mds_cnt <= 1) {
        logging(LOG_WARN, "we need at least 2 mds !, fs not mk");
        return;
    }
    mkfs_send_request(mds[0], mds[1]);
}

#undef RST_FOUND


static struct fuse_lowlevel_ops sfs_ll_op = {
    /*.init       = sfs_ll_init, */
    .lookup =   sfs_ll_lookup,
    .getattr =  sfs_ll_getattr,
    .readdir =  sfs_ll_readdir,
    .open =     sfs_ll_open,
    .read =     sfs_ll_read,
    .write =    sfs_ll_write,
    .setattr =  sfs_ll_setattr,
    .flush =    sfs_ll_flush,
    .create =   sfs_ll_create_async,
    /*.create =   sfs_ll_create,*/
    .mkdir =    sfs_ll_mkdir,
    .unlink =   sfs_ll_unlink,
    .rmdir =    sfs_ll_unlink,
    .statfs =   sfs_ll_statfs,
    .release =  sfs_ll_release,

    .symlink =  sfs_ll_symlink,
    .readlink = sfs_ll_readlink,
    .access =   sfs_ll_access,

};

void usage(const char *appname)
{

}

void onexit()
{

}

extern struct evrpc_pool *pool;
int main(int argc, char *argv[])
{
    init_app(argc, argv, "client");
    mds_conn_init();
    attr_cache_init();
    if (!find_root()) {
        sfs_mkfs();
        find_root();
    }
    http_client_init();

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_chan *ch;
    char *mountpoint = "/tmp/a";
    int err = -1;

    //if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
    if (fuse_parse_cmdline(&args, NULL, NULL, NULL) != -1 &&
        (ch = fuse_mount(mountpoint, &args)) != NULL) {
        struct fuse_session *se;

        se = fuse_lowlevel_new(&args, &sfs_ll_op, sizeof(sfs_ll_op), NULL);
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
