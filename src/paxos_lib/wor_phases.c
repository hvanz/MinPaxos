#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/space.h" //paxos_space_is_written()
#include "wor_steps.h"

// Each preemption increases the proposal number, so the following capture phase
// will have a chance to run ok.
// NB: If the WOR is ALREADYWRITTEN, then it cannot be captured.
// Return OK, ALREADYWRITTEN, TIMEOUT, ERROR.
int wor_capture (uint addr) {
    int ret, count;

    count = MAX_RETRIES;
    ret = STATUS_PREEMPTED;
    while (0 < count && ret == STATUS_PREEMPTED) {
        if (count < MAX_RETRIES) printf("WOR: retrying capture on addr=%u\n", addr);
        ret = wor_capture_send(addr);
        if (ret == STATUS_OK) {
            ret = wor_capture_receive(addr); // ret = OK or PREEMPTED
        } // else ret = ALREADYWRITTEN
        count--;
    }

    if (count == 0 || ret == STATUS_PREEMPTED) {
        return STATUS_TIMEOUT;
    } else {
        return ret; // OK, ALREADYWRITTEN or ERROR
    }
}

// NB: If doesn't matter if the WOR is ALREADYWRITTEN; wor_write_send will
// select the already-written-value to perform this step.
// Return OK or PREEMPTED.
int wor_write (uint addr, cmd_t cmd0, uint cmd1) {
    int ret;
    wor_write_send(addr, cmd0, cmd1);
    ret = wor_write_receive(addr);
    return ret; // OK or PREEMPTED
}
