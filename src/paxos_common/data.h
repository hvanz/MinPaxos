#ifndef _DATA_H_
#define _DATA_H_

#include <sys/types.h>

#if !defined(uint)
    #define uint unsigned int
#endif

#if !defined(NULL)
    #define NULL ((void*)0)
#endif

#if !defined(size_t)
    #define size_t unsigned int
#endif

/* Command types */
typedef enum {
    CMD_UNDEF,
    CMD_SEGMENT_OWNER,
    CMD_JUNK,
    CMD_RECONFIGURE,
    CMD_REPCOUNTER_INC,  //Replicated counter; increment by x
    CMD_REPCOUNTER_READ  //Replicated counter; read value
} cmd_t;

void print_cmd (cmd_t cmd0, uint cmd1);

// A node is a client or an acceptor; valid ids are > 0; id = 0 means unspecified
uint node_id_get (void);
void node_id_set (uint x);

#endif /* _DATA_H_ */
