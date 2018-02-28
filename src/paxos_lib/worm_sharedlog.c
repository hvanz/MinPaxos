#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "../paxos_common/space.h" // paxos_space_is_written, ..
#include "paxos_view.h" // client_view_init
#include "paxos_comm.h" // paxos_comm_init
//#include "wor.h"
#include "wos.h"
#include "sharedlog_indices.h"

uint current_segment;

void current_segment_set (uint s) { current_segment = s; }
void current_segment_inc () { current_segment++; } // TODO: check limits
uint current_segment_get () { return current_segment; }

/******************************************************************************/

void update_segment () {
    uint s;
    s = current_segment_get();
    while (wos_alloc(s) != STATUS_OK) {
        current_segment_inc();
    }
}

// Initialize client_id and WOAS.
void worm_sharedlog_init (uint client_id) {
    num_acceptors_set(MIN_ACCEPTORS);
    client_view_init();
    node_id_set(client_id);
    paxos_lib_comm_init(client_id);
    sharedlog_indices_init();
    current_segment_set(0);
    update_segment();
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
uint worm_sharedlog_append (cmd_t cmd0, uint cmd1) {
    //uint addr;
    int res;
    uint seg;

    // TODO: add cmd to a buffer?
    // TODO: put a lock on first_blank_addr?

    // Every call to this function should be executed with a different addr.
    //addr = fetch_and_inc_first_blank_addr(); //TODO: make it atomic
//wos_read (current_segment, current_offset);
    seg = current_segment_get();
    res = wos_append(seg, cmd0, cmd1);
    // res = wor_capture_and_write(addr, cmd0, cmd1);
    switch (res) {
    case STATUS_NOTOWNER: {
        // Again, update indices just in case some othe client have added entries.
        //update_first_unwritten_addr();
        current_segment_set(seg+1);
        update_segment();
        return worm_sharedlog_append(cmd0,cmd1);
        break; }
    // case STATUS_ALREADYWRITTEN: {
    //     // Again, update just in case some othe client have added entries.
    //     update_first_unwritten_addr();
    //     // Retry with a new address
    //     return sharedlog_append(cmd0,cmd1);
    //     break; }
    case STATUS_OK: {
        // At least one log entry (addr) was written. Other clients may have as
        // well added new entries: when update, take into account all of them.
        //update_first_unwritten_addr();
        return STATUS_OK;
        break; }
    default: //EOS_ERROR, TIMEOUT
        return STATUS_UNKNOWNERROR;
    }
}

// cmd_t worm_space_read_cmd0 (uint addr) {
//     if (paxos_space_is_written(addr)) {
//         return paxos_space_get_vcmd0(addr);
//     } else {
//         return CMD_UNDEF;
//     }
// }
//
// uint worm_space_read_cmd1 (uint addr) {
//     if (paxos_space_is_written(addr)) {
//         return paxos_space_get_vcmd1(addr);
//     } else {
//         return 0; // irrelevant value
//     }
// }

/******************************************************************************/

// sharedlog_getnext return variables
cmd_t sharedlog_next_cmd0;
uint sharedlog_next_cmd1;

cmd_t worm_sharedlog_getnext_cmd0 () { return sharedlog_next_cmd0; }
uint worm_sharedlog_getnext_cmd1 () { return sharedlog_next_cmd1; }
void worm_sharedlog_getnext_cmd0_set (cmd_t x) { sharedlog_next_cmd0 = x; }
void worm_sharedlog_getnext_cmd1_set (uint x) { sharedlog_next_cmd1 = x; }

/******************************************************************************/

// The client can retrieve written commands in the prefix [0..first_unwritten_addr-1] one by one.
// Commands already returned are in [0..first_unexecuted_addr-1].
// This function returns the next command in the prefix not yet retuned to the client.
// The client is responsible for executing the command.
// Initially, first_unexecuted_addr = first_unwritten_addr = 0, that is, there
// is no written value in the log that can be returned.
int worm_sharedlog_getnext () {
    uint off, seg;
    int res;
    seg = current_segment_get();
    off = first_unexecuted_addr_get();
    res = wos_read(seg, off);
    if (res == STATUS_OK && off < first_unwritten_addr_get()) {
        worm_sharedlog_getnext_cmd0_set(wos_read_cmd0_get());
        worm_sharedlog_getnext_cmd1_set(wos_read_cmd1_get());
        // worm_sharedlog_getnext_cmd0_set(worm_space_read_cmd0(addr));
        // worm_sharedlog_getnext_cmd1_set(worm_space_read_cmd1(addr));
        increment_first_unexecuted_addr();
        return STATUS_OK;
    } else {
        return STATUS_UNKNOWNERROR;
    }
}

// int sharedlog_getnext () {
//     uint offset = first_unexecuted_addr_get();
//     int res;
//     res = wos_read(current_segment, offset);
//     if (res == STATUS_OK) {
//         //if (addr < first_unwritten_addr_get()) {
//         sharedlog_getnext_cmd0_set(wos_read_cmd0_get());
//         sharedlog_getnext_cmd1_set(wos_read_cmd1_get());
//         increment_first_unexecuted_addr();
//         return STATUS_OK;
//     } else {
//         return STATUS_UNKNOWNERROR;
//     }
// }
