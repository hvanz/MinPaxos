#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
// #include "woas.h" // woas_init, woas_consensus, woas_read_type, woas_read_arg1
#include "../paxos_common/space.h" // paxos_space_init_all
#include "paxos_view.h" // client_view_init
#include "paxos_comm.h" // paxos_comm_init
#include "wor.h"
#include "sharedlog_indices.h"

// Initialize client_id and WOAS.
void sharedlog_init (uint client_id) {
    num_acceptors_set(MIN_ACCEPTORS);
    node_id_set(client_id);
    sharedlog_indices_init();
    // woas_init(client_id);
    client_view_init();
    paxos_lib_comm_init(client_id);

    paxos_space_init_all();
    int i;
    for (i = 0; i < LOG_SIZE; i++) {
        paxos_space_set_prop(i,1);  // initial proposal number is 1
    }
}

// Try to append command to the next empty entry in the log.
// 1. Find first unexecuted entry in local log (there could be some acceptors that have already chosen a value)
// 2. If capture phase returns "accepted value", do accept phase to finish choosing that value.
// Question: what if when trying to capture the proposal number is higher than
//    that of the accepted value? then accepted value would be overwritten.
// 3. Start again on the next entry
// Another approach: if there is some partially written value v (i.e. accepted
//   by a minority of acceptors) at slot i, and some committed value w at slot j,
//   where i < j, then either v is eventually committed or no-op is eventually
//   committed.
uint sharedlog_append (cmd_t cmd0, uint cmd1) {
    uint addr;
    int res;

    // TODO: add cmd to a buffer?
    // TODO: put a lock on first_blank_addr?

    // Every call to this function should be executed with a different addr.
    addr = fetch_and_inc_first_blank_addr(); //TODO: make it atomic
    res = wor_capture_and_write(addr, cmd0, cmd1);
    switch (res) {
    case STATUS_OK: {
        // At least one log entry (addr) was written. Other clients may have as
        // well added new entries: when update, take into account all of them.
        update_first_unwritten_addr();
        return STATUS_OK;
        break; }
    case STATUS_ALREADYWRITTEN: {
        // Again, update just in case some othe client have added entries.
        update_first_unwritten_addr();
        // Retry with a new address
        return sharedlog_append(cmd0,cmd1);
        break; }
    default:
        return STATUS_UNKNOWNERROR;
    }
}

cmd_t paxos_space_read_cmd0 (uint addr) {
    if (paxos_space_is_written(addr)) {
        return paxos_space_get_vcmd0(addr);
    } else {
        return CMD_UNDEF;
    }
}

uint paxos_space_read_cmd1 (uint addr) {
    if (paxos_space_is_written(addr)) {
        return paxos_space_get_vcmd1(addr);
    } else {
        return 0; // irrelevant
    }
}

// The client can retrieve written commands in the prefix [0..first_unwritten_addr-1] one by one.
// Commands already returned are in [0..first_unexecuted_addr-1].
// This function returns the next command in the prefix not yet retuned to the client.
// The client is responsible for executing the command.
// Initially, first_unexecuted_addr = first_unwritten_addr = 0, that is, there
// is no written value in the log that can be returned.
int sharedlog_getnext (cmd_t * cmd0, uint * cmd1) {
    uint addr = first_unexecuted_addr_get();
    if (addr < first_unwritten_addr_get()) {
        *cmd0 = paxos_space_read_cmd0(addr);
        *cmd1 = paxos_space_read_cmd1(addr);
        increment_first_unexecuted_addr();
        return STATUS_OK;
    } else {
        return STATUS_UNKNOWNERROR;
    }
}
