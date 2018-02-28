#include <stdio.h>
#include <stdlib.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "../paxos_common/space.h"

// Returned pair by the function wor_select_value(); cmd0 = cmd id, cmd1 = arg1
static cmd_t wor_selected_cmd0 = CMD_UNDEF;
static uint wor_selected_cmd1 = 0;

cmd_t wor_selected_cmd0_get () { return wor_selected_cmd0; }
uint wor_selected_cmd1_get () { return wor_selected_cmd1; }
void wor_selected_cmd0_set (cmd_t x) { wor_selected_cmd0 = x; }
void wor_selected_cmd1_set (uint x) { wor_selected_cmd1 = x; }

// Retrieve selected value from space and return whether it is undefined.
// The value selected from the capture phase (cmd0,cmd1) is undefined if it is
// equal to (CMD_UNDEF,0) and the WOR is not written.
int wor_selected_value_is_undef (uint addr) {
    int is_written;
    is_written = paxos_space_is_written(addr);
    wor_selected_cmd0 = paxos_space_get_vcmd0(addr);
    wor_selected_cmd1 = paxos_space_get_vcmd1(addr);
    return (!is_written && wor_selected_cmd0 == CMD_UNDEF && wor_selected_cmd1 == 0);
}

// PaxosPluscal step 2b
// If the value selected from the capture phase is (CMD_UNDEF,0), then command
// the acceptors to chose (cmd0,cmd1).
void wor_select_value (uint addr, cmd_t cmd0, uint cmd1) {
    cmd_t old_cmd0;
    uint old_cmd1;
    if (wor_selected_value_is_undef(addr)) {
        wor_selected_cmd0_set(cmd0);
        wor_selected_cmd1_set(cmd1);
    } else {
        old_cmd0 = paxos_space_get_vcmd0(addr);
        old_cmd1 = paxos_space_get_vcmd1(addr);
        wor_selected_cmd0_set(old_cmd0);
        wor_selected_cmd1_set(old_cmd1);
    }
}
