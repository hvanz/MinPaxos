#ifndef _PAXOS_VIEW_H_
#define _PAXOS_VIEW_H_

#include "../paxos_common/messages.h"

#define MIN_ACCEPTORS 3
#define MAX_ACCEPTORS 7

void num_acceptors_set (uint n);
uint num_acceptors_get ();

void client_view_init ();
uint client_view_addr (uint acc_id);
uint client_view_port (uint acc_id);

#endif // end of #ifndef _PAXOS_VIEW_H_
