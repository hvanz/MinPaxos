//#include <stdio.h>
#include "../paxos_common/paxos.h"
#include "../paxos_common/messages.h"
#include "paxos_view.h"

//extern uint htons (uint x);

uint paxos_num_acceptors;

void num_acceptors_set (uint n) {
    if (MIN_ACCEPTORS <= n) {
        if (n <= MAX_ACCEPTORS) {
            paxos_num_acceptors = n;
        } else {
            paxos_num_acceptors = MAX_ACCEPTORS;
        }
    } else {
        paxos_num_acceptors = MIN_ACCEPTORS;
    }
}
uint num_acceptors_get () { return paxos_num_acceptors; }

/******************************************************************************/

// A view is a configuration of acceptors' IP address and port.
uint paxos_view_addr[MAX_ACCEPTORS];
uint paxos_view_port[MAX_ACCEPTORS];

void client_view_init () {
    int acc_ix;
    uint acc_port, acc_addr;
    acc_ix = num_acceptors_get() - 1;
    while (0 <= acc_ix) {
        paxos_view_addr[acc_ix] = INET_ADDR_NET;
        paxos_view_port[acc_ix] = htons(ACCEPTORS_PORT + acc_ix + 1);
        acc_ix--;
    }
}

uint client_view_addr (uint acc_ix) {
    return paxos_view_addr[acc_ix];
}

uint client_view_port (uint acc_ix) {
    return paxos_view_port[acc_ix];
}
