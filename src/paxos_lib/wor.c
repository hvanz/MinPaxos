#include <stdio.h>
#include <stdlib.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "../paxos_common/space.h"
#include "wor_phases.h"
#include "wor_helper.h"

// Return values of wor_read()
cmd_t wor_read_cmd0;
uint wor_read_cmd1;

cmd_t wor_read_cmd0_get () { return wor_read_cmd0; }
uint wor_read_cmd1_get () { return wor_read_cmd1; }
void wor_read_cmd0_set (cmd_t x) { wor_read_cmd0 = x; }
void wor_read_cmd1_set (uint x) { wor_read_cmd1 = x; }

/******************************************************************************/

void wor_init (uint addr) {
    paxos_space_init(addr);
    //wor_read_cmd0_set(CMD_UNDEF);
    //wor_read_cmd1_set(0);
    //wor_selected_cmd0_set(CMD_UNDEF);
    //wor_selected_cmd1_set(0);
}

// This function tries to reach consensus on a value (cmd0,cmd1), for some given
// address addr. It orchestrates a 'capture' phase followed by an 'write' phase.
// If any of both phases is preempted, it retries starting from the beginning
// (capture phase). A preemption increases the proposal number, so the following
// capture phase won't be repeated.
// If succesful, it sets log[addr].is_written = TRUE
// Return OK, ALREADYWRITTEN, TIMEOUT, ERROR
// ERROR = "no more proposal numbers available"
int wor_capture_and_write (uint addr, cmd_t cmd0, uint cmd1) {
    int ret;
    uint count;

    count = MAX_RETRIES;
    ret = STATUS_PREEMPTED;
    while (0 < count && ret == STATUS_PREEMPTED) {
        if (count < MAX_RETRIES) printf("WOR: retrying on addr=%u\n", addr);
        ret = wor_capture(addr);
        if (ret == STATUS_TIMEOUT) {
            count = 0;
        } else if (ret == STATUS_OK) {
            ret = wor_write(addr, cmd0, cmd1); // ret = OK or PREEMPTED
            count--;
        } // else ret = ALREADYWRITTEN;
    }
    if (ret == STATUS_OK) printf("[n%d] WOR: command chosen: ", node_id_get()); print_cmd(cmd0, cmd1); printf("\n");

    if (count == 0 || ret == STATUS_PREEMPTED) {
        return STATUS_TIMEOUT;
    } else {
        return ret; // OK, ALREADYWRITTEN, ERROR
    }
}


// "The read protocol is a phase 1a message; if it encounters a half-written
// quorum, it completes the write. Each wormserver maintains a map from WOR
// identifiers to the acceptor state for that single-shot Paxos instance. If an
// entry does not exist in this map, the status of the WOR is unwritten."
// Return OK, ALREADYWRITTEN, UNWRITTEN, TIMEOUT (when MAX_READ_RETRIES reached)
// If OK, set return variable to (cmd,arg1); otherwise, set it to (CMD_UNDEF,0).
int wor_read (uint addr) {
    cmd_t cmd0;
    uint cmd1;
    int ret;
    if (paxos_space_is_written(addr)) {
        printf("WOR: value already written.\n");
        cmd0 = paxos_space_get_vcmd0(addr);
        cmd1 = paxos_space_get_vcmd1(addr);
        wor_read_cmd0_set(cmd0);
        wor_read_cmd1_set(cmd1);
        return STATUS_ALREADYWRITTEN;
    } else {
        // WOR is unwritten or partially-written (in the middle of a write
        // process by another proposer) or already written but not yet learnt
        // locally.
        ret = wor_read_helper(addr);
        if (ret == STATUS_OK || ret == STATUS_ALREADYWRITTEN) {
            // assert(paxos_space_is_written(addr));
            cmd0 = paxos_space_get_vcmd0(addr);
            cmd1 = paxos_space_get_vcmd1(addr);
            wor_read_cmd0_set(cmd0);
            wor_read_cmd1_set(cmd1);
            printf("[n%d] WOR: command read: ", node_id_get()); print_cmd(wor_read_cmd0, wor_read_cmd1); printf("\n");
            return STATUS_OK;
        } else { // ret = UNWRITTEN or TIMEOUT or ERROR
            wor_read_cmd0_set(CMD_UNDEF);
            wor_read_cmd1_set(0); // irrelevant
            return ret;
        }
    }
}
