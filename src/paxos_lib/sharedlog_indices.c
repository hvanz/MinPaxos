#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/space.h" // paxos_space_is_written

/******************************************************************************/

// The log prefix [0..first_unexecuted_addr-1] are those values that were
// returned to the client. The client/data structure is responsible for
// executing these values in its state machine.
uint first_unexecuted_addr;
// The log prefix [0..first_unwritten_addr-1] are those values that were
// accepted and written by the acceptors and cannot be modified.
uint first_unwritten_addr;
// The log prefix [0..first_blank_addr-1] are those values that are already
// written or in the process of being written by the acceptors.
uint first_blank_addr;

/* Prefixes invariant:
 * 0 <= first_unexecuted_addr < first_unwritten_addr <= first_blank_addr < LOG_SIZE
 *
 *  ---------------------------------------------------------
 *  |   | X | X | X | A | A | A | A | ? | ? | ? |   |   |   |  log
 *  ---------------------------------------------------------
 *    0   1           ^               ^           ^       LOG_SIZE-1
 *        executed    unexecuted &    unwritten   blank
 *                    written
 */

uint first_unexecuted_addr_get () { return first_unexecuted_addr; }
uint first_unwritten_addr_get () { return first_unwritten_addr; }

// log[0] is unused
void sharedlog_indices_init () {
    first_unexecuted_addr = 0;
    first_unwritten_addr = 1;
    first_blank_addr = 1;
}

//TODO: make it atomic
uint fetch_and_inc_first_blank_addr () {
    if (first_blank_addr < LOG_SIZE) {
        first_blank_addr++;
        return first_blank_addr - 1;
    } else {
        // TODO: do something
        perror("log is full");
        return 0;
    }
}

void update_first_unwritten_addr () {
    while (first_unwritten_addr < LOG_SIZE &&
           paxos_space_is_written(first_unwritten_addr)) {
        first_unwritten_addr++;
    }
    if (first_unwritten_addr == LOG_SIZE) {
        //TODO: do something
        perror("log is full");
        first_unwritten_addr = 0;
    }
}

void increment_first_unexecuted_addr () {
    if (first_unexecuted_addr < LOG_SIZE) {
        first_unexecuted_addr++;
    } else {
        // TODO: do something
        perror("log is full");
        first_unexecuted_addr = 0;
    }
}
