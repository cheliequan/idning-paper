sbin_PROGRAMS=test

AM_CPPFLAGS=-std=c99 -I$(top_srcdir)/common -DMFSMAXFILES=10000 -D_USE_PTHREADS $(PTHREAD_CPPFLAGS) -DAPPNAME=test
AM_LDFLAGS=$(PTHREAD_LIBS) $(ZLIB_LIBS)

test_SOURCES= \
	network_test.c
	../common/network.c ../common/network.h \
	../common/log.c ../common/log.h 

#mfschunkserver_LDADD=../mfscommon/libmfscommon.a
#mfschunkserver_CFLAGS=$(PTHREAD_CFLAGS)
