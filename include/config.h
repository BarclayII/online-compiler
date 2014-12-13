#ifndef _CONFIG_H
#define _CONFIG_H

#ifndef ROOTDIR
#define ROOTDIR	    "ocmpl"
#endif

#ifndef UID_CMPL
#define UID_CMPL	1000
#endif

#ifndef UID_SRVR
#define UID_SRVR	1000
#endif

#ifndef SRVR_PORT
#define SRVR_PORT	6700
#endif

#ifndef PATH_SEP
#define PATH_SEP	"/"
#endif

#ifndef OUTEXEC_CONFIG
#define OUTEXEC_CONFIG	".outexec$"
#endif

#ifndef LDFLAGS_CONFIG
#define LDFLAGS_CONFIG	".ldflags$"
#endif

#ifndef DFL_UMASK
#define DFL_UMASK	0644
#endif

#ifndef DFL_UMASK_DIR
#define DFL_UMASK_DIR	0755
#endif

#ifndef CFLAGS_CONFIG
#define CFLAGS_CONFIG	".cflags$"
#endif

#ifndef LEXOUT_CONFIG
#define LEXOUT_CONFIG	".lexout$"
#endif

#ifndef YACCOUT_CONFIG
#define YACCOUT_CONFIG	".yaccout$"
#endif

#ifndef SRVR_MAXCONN
#include <sys/socket.h>
#define SRVR_MAXCONN	SOMAXCONN
#endif

#ifndef SRVR_MAXTASK
#define SRVR_MAXTASK	SRVR_MAXCONN
#endif

#ifndef SRVR_MAXTHREAD
#define SRVR_MAXTHREAD	SRVR_MAXCONN
#endif

#endif
