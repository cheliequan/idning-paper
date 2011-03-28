#include<protocol.h>

    EVRPC_GENERATE(rpc_ping, ping, pong)
    EVRPC_GENERATE(rpc_stat, stat_request, stat_response)
    EVRPC_GENERATE(rpc_ls, ls_request, ls_response)

    EVRPC_GENERATE(rpc_mknod, mknod_request, mknod_response)
    EVRPC_GENERATE(rpc_unlink, unlink_request, unlink_response)
    EVRPC_GENERATE(rpc_statfs, statfs_request, statfs_response)
    EVRPC_GENERATE(rpc_lookup, lookup_request, lookup_response)

    EVRPC_GENERATE(rpc_setattr, setattr_request, setattr_response)
    EVRPC_GENERATE(rpc_mkfs, mkfs_request, mkfs_response)

/*EVRPC_GENERATE(rpc_, _request, _response)*/
/*EVRPC_GENERATE(rpc_, _request, _response)*/
/**
 * this should be in rpc_gen.py 
 * but I have no time to do this
 */
/*void file_stat_init(struct file_stat * stat){*/
    /*memset(stat, 0, sizeof(struct file_stat)); */
    /*stat->base = &__file_stat_base; */
/*}*/
