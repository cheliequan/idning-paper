
#include "app.h"
int usage(char * appname);


int init_app(int argc,char **argv, char * appname) {
    //char *wrkdir;
    int ch;
    //uint8_t runmode;
    int rundaemon,logundefined;
    struct rlimit rls;

    char cfgfile[999];
    sprintf(cfgfile, "etc/%s.cfg", appname);
    rundaemon=1;
    //runmode = RM_RESTART;
    logundefined=0;
    //appname = argv[0];

    while ((ch = getopt(argc, argv, "vduc:h?")) != -1) {
        switch(ch) {
            case 'd':
                rundaemon=0;
                break;
            case 'c':
                strncpy(cfgfile, optarg, sizeof(cfgfile));
                break;
            case 'u':
                logundefined=1;
                break;
            case 'v':
                printf("version: %s\n",APP_VERSION);
                exit(0);
            default:
                usage(appname);
                exit(0);
        }
    }
    argc -= optind;
    argv += optind;
    if (argc==1) {
        if (strcasecmp(argv[0],"start")==0) {
            //runmode = RM_START;
        } else if (strcasecmp(argv[0],"stop")==0) {
            //runmode = RM_STOP;
        } else if (strcasecmp(argv[0],"restart")==0) {
            //runmode = RM_RESTART;
        } else if (strcasecmp(argv[0],"reload")==0) {
            //runmode = RM_RELOAD;
        } else {
            usage(appname);
            return 1;
        }
    } else if (argc!=0) {
        usage(appname);
        return 1;
    }

    if (cfg_load(cfgfile,logundefined)==0) {
        fprintf(stderr,"can't load config file: %s - using defaults\n",cfgfile);
    }

    rls.rlim_cur = MAX_FILES;
    rls.rlim_max = MAX_FILES;
    if (setrlimit(RLIMIT_NOFILE,&rls)<0) {

        fprintf(stderr,"can't change open files limit to %u\n",MAX_FILES);
    }
    /*

    wrkdir = cfg_getstr("DATA_PATH",DATA_PATH);
    fprintf(stderr,"working directory: %s\n",wrkdir);

    if (chdir(wrkdir)<0) {
        fprintf(stderr,"can't set working directory to %s\n",wrkdir);
        syslog(LOG_ERR,"can't set working directory to %s",wrkdir);
        return 1;
    }
    free(wrkdir);

    if ((runmode==RM_START || runmode==RM_RESTART) && rundaemon) {
        msgfd = makedaemon();
    } else {
        if (runmode==RM_START || runmode==RM_RESTART) {
            set_signal_handlers();
        }
        msgfd = fdopen(dup(STDERR_FILENO),"w");
        setvbuf(msgfd,(char *)NULL,_IOLBF,0);
    }

    umask(027);
*/
    return 0;
}
