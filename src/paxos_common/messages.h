#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "paxos.h"
#include "data.h"

/* Message types */
typedef enum {MSG_CAPTURE, MSG_CAPTURE_ACK, MSG_ACCEPT, MSG_ACCEPT_ACK} msg_type_t;

/* All four type of messages fit in the following format. */
typedef struct _msg_t {
	msg_type_t type;
	uint       from;
	uint 	   addr; //log index
	uint       prop; //proposal number
	uint       vprop; // vote proposal
	cmd_t      vcmd0; // vote command0
	uint       vcmd1; // vote command1
} msg_t;

void print_msg (msg_t * m);

#endif /* _MESSAGES_H_ */
