#include "paxos.h"
#include "data.h"

struct paxos_space_entry {
    uint prop;   // (Pa) Highest proposal number seen by the acceptor.
    uint vprop;  // (Aa) Highest proposal number...
    cmd_t vcmd0; // (Va)   and value (command type and argument) in
    uint vcmd1;  // (Va)   which acceptor has voted for.
    uint is_written;
};

static struct paxos_space_entry paxos_space[LOG_SIZE];

// Another approach: use only one variable prop for locking the whole log.
//  Most of the time, only one client/proposer is interacting with the acceptors.
//  In case of conflict between two or more proposers, it would be the same
//  livelock situation as in single-paxos. In practice, eventually a leader
//  would be found and the system stabilized.

//TODO: initialize from persitent storage
void paxos_space_init (uint addr) {
    paxos_space[addr].prop = PROP_UNDEF;
    paxos_space[addr].vprop = PROP_UNDEF;
    paxos_space[addr].vcmd0 = CMD_UNDEF;
    paxos_space[addr].vcmd1 = 0;
    paxos_space[addr].is_written = 0;
}

void paxos_space_init_all () {
    int i;
    for (i = 0; i < LOG_SIZE; i++) {
        paxos_space_init(i);
    }
}

uint paxos_space_get_prop (uint addr) { return paxos_space[addr].prop; }
uint paxos_space_get_vprop (uint addr) { return paxos_space[addr].vprop; }
cmd_t paxos_space_get_vcmd0 (uint addr) { return paxos_space[addr].vcmd0; }
uint paxos_space_get_vcmd1 (uint addr) { return paxos_space[addr].vcmd1; }
uint paxos_space_is_written (uint addr) { return paxos_space[addr].is_written; }

//TODO: update variables in persistent storage
void paxos_space_set_prop  (uint addr, uint x) { paxos_space[addr].prop = x; }
void paxos_space_set_vprop (uint addr, uint x) { paxos_space[addr].vprop = x; }
void paxos_space_set_vcmd0 (uint addr, cmd_t x) { paxos_space[addr].vcmd0 = x; }
void paxos_space_set_vcmd1 (uint addr, uint x) { paxos_space[addr].vcmd1 = x; }
void paxos_space_set_written_on (uint addr) { paxos_space[addr].is_written = 1; }
void paxos_space_set_written_off (uint addr) { paxos_space[addr].is_written = 0; }
