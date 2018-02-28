#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
//#include "../paxos_lib/sharedlog.h"
#include "../paxos_lib/worm_sharedlog.h"
#include "repcounter.h"

/******************************************************************************/

struct repcounter {
    uint value;
};
struct repcounter counter;

void counter_value_set (uint x) { counter.value = x; }
uint counter_value_get (void) { return counter.value; }

void counter_perform_cmd (cmd_t cmd0, uint cmd1) {
    printf("[n%d] RC: perform cmd ", node_id_get()); print_cmd(cmd0,cmd1); printf("\n");
    switch (cmd0) {
    case CMD_REPCOUNTER_READ:
        break;
    case CMD_REPCOUNTER_INC:
        counter_value_set(counter_value_get() + cmd1);
        break;
    default: ;
    }
}

void repcounter_execute_all_pending () {
    cmd_t cmd0; // command type
    uint cmd1;  // command argument
    while (worm_sharedlog_getnext() == STATUS_OK) {
        cmd0 = worm_sharedlog_getnext_cmd0();
        cmd1 = worm_sharedlog_getnext_cmd1();
        counter_perform_cmd(cmd0, cmd1);
    }
}

/******************************************************************************/

/* Initialize replicated counter.
 * Create thread to receive messages from acceptors.
 * Read replicated value and record it locally.
 */
void repcounter_init (uint client_id) {
    uint addr;
    counter_value_set(0);
    worm_sharedlog_init(client_id);
}

int repcounter_read () {
    // After appending a 'read' command, the log local copy will be up-to-date.
    if (worm_sharedlog_append(CMD_REPCOUNTER_READ,0) == STATUS_OK) {
        repcounter_execute_all_pending();
        // After executing all pending written commands, it is safe to return
        // the current state of the replicated counter.
        return counter_value_get();
    } else {
        return -1;
    }
}

/* Increase replicated counter by x units.
 * Creates a CMD_REPCOUNTER_INC command and appends it to the shared log.
 * If ok, updates the local counter.
 */
int repcounter_inc (uint x) {
    if (worm_sharedlog_append(CMD_REPCOUNTER_INC,x) == STATUS_OK) {
        repcounter_execute_all_pending();
        return STATUS_OK;
    } else {
        return STATUS_UNKNOWNERROR;
    }
}

void repcounter_destroy (void) {
    //pthread_cancel(thread);
}
