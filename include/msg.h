#ifndef _MSG_H
#define _MSG_H

struct msghdr {
	int sender;
	int receiver;
	int info_level;
	int msgtype;
	union {
		int errno;	/* MT_INTERN */
	} un;
	int payload_len;
};

#define MT_INTERN	1	/* Component internal error */

#endif
