#ifndef _PAXOS_COMM_H_
#define _PAXOS_COMM_H_

#include "../paxos_common/messages.h"

// Client communication
int paxos_lib_comm_init (uint client_id);
// void paxos_lib_comm_close (); //not used

// The arguments are all the fields in a message.
void paxos_broadcast_msg (msg_type_t type, uint addr, uint prop,
                          uint vprop, cmd_t vote_cmd0, uint vote_cmd1);

#endif // end of #ifndef _PAXOS_COMM_H_
