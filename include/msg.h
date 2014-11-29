#ifndef _MSG_H
#define _MSG_H

struct msghdr {
	int sender;
	int recver;
	int info_level;
	union {
	} un;
	int payload_len;
};

#endif
