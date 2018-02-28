#include <stdio.h>
#include <stdlib.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "../paxos_common/space.h"
#include "wor_selector.h"
#include "wor_phases.h"

// Return OK, ALREADYWRITTEN, UNWRITTEN, TIMEOUT (when MAX_READ_RETRIES reached)
int wor_read_helper (uint addr) {
    int ret, count, value_is_undef;
    cmd_t cmd0;
    uint cmd1;
    count = MAX_RETRIES;
    ret = STATUS_PREEMPTED;
    value_is_undef = FALSE;
    while (0 < count && ret == STATUS_PREEMPTED && !value_is_undef) {
        if (count < MAX_RETRIES) printf("WOR: retrying on addr=%u\n", addr);
        ret = wor_capture(addr); // ret = OK, ALREADYWRITTEN or TIMEOUT.
        if (ret == STATUS_TIMEOUT) { // WOR could not be captured.
            count = 0;
        } else if (ret == STATUS_OK) { // WOR was captured.
            value_is_undef = wor_selected_value_is_undef(addr);
            if (!value_is_undef) { // There is a (partially-)written value; continue writing that value.
                cmd0 = wor_selected_cmd0_get();
                cmd1 = wor_selected_cmd1_get();
                ret = wor_write(addr, cmd0, cmd1); //ret = OK or PREEMPTED
            } else { // There is no written value in acceptors.
                ret = STATUS_UNWRITTEN;
            }
        } // else ret = ALREADYWRITTEN
        count--;
    }
    return ret;
}
