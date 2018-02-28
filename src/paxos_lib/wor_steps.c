#include <stdio.h>
#include <stdlib.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "../paxos_common/space.h"
#include "paxos_comm.h"
#include "paxos_view.h" // num_acceptors_get()
#include "wor_quorums.h"
#include "wor_selector.h"
#include "wor_handler.h"

// PaxosPluscal step P1
// Initilise WOR and send a 'capture' message to all acceptors.
// Return OK, ALREADYWRITTEN, ERROR.
int wor_capture_send (uint addr) {
    // printf("WOR: capture phase on addr=%u...\n",addr);
    uint prop;

    if (paxos_space_is_written(addr)) {
        printf("[n%d] WOR: addr=%u is already written\n",node_id_get(),addr);
        return STATUS_ALREADYWRITTEN;
    }

    // Reset voted ballot, voted value, quorum counters.
    paxos_space_set_vprop(addr, PROP_UNDEF);
    paxos_space_set_vcmd0(addr, CMD_UNDEF);
    paxos_space_set_vcmd1(addr, 0);
    wor_quorums_capture_reset(addr);
    wor_quorums_write_reset(addr);

    // Initially, the proposal number is PROP_UNDEF, which is smaller than
    // any other proposal. Then, if the proposal number is small, the capture
    // phase will be preempted by some acceptor, which will transfer back its
    // minimum proposal number to be used next. Eventually a proposal number big
    // enough will be found. This mechanism avoids computing a fresh number
    // between all proposers.
    prop = wor_get_next_proposal(addr);
    if (prop != PROP_UNDEF) {
        paxos_broadcast_msg(MSG_CAPTURE, addr, prop, PROP_UNDEF, CMD_UNDEF, 0);
        return STATUS_OK;
    } else {
        return STATUS_ERROR;
    }
}

// PaxosPluscal step P2a
// Wait until (1) the process is preempted by some ack message with higher
// proposal number, or (2) there is a majority of acceptors that sent ack
// messages, or (3) the number of received messages is the number of acceptors.
// NB: It is an assumption that at least a majority of acceptors do not fail
// and they will eventually reply. Thus, if no messages are dropped, the
// loop terminates.
// NB: The loop does not wait for all the acceptors; it cuts off when
// messages from a majority arrive.
// NB: Messages that are not for current [addr] are discarded; still they count
// as a message.
// Return OK, PREEMPTED
int wor_capture_receive (uint addr) {
    int count, ret;
    count = num_acceptors_get();
    ret = STATUS_OK;
    while (0 < count && ret == STATUS_OK && !wor_quorums_capture_is_quorum(addr)) {
        ret = wor_handle_capture_ack(addr); // ret = OK or PREEMPTED
        count--;
    }

    if (count == 0) {
        ret = STATUS_PREEMPTED; // either no good messages or no quorum
    }
    return ret;
}

// PaxosPluscal step P2b-- Select and start Accept phase.
// 1. Select value, either the one passed as parameter or the value recovered
//    from the capture phase.
// 2. Send 'accept' message to all acceptors, with selected value.
void wor_write_send (uint addr, cmd_t cmd0, uint cmd1) {
    uint prop;

    wor_select_value(addr, cmd0, cmd1);
    cmd0 = wor_selected_cmd0_get();
    cmd1 = wor_selected_cmd1_get();

    if (cmd0 != PROP_UNDEF) {
        prop = paxos_space_get_prop(addr);
        paxos_broadcast_msg(MSG_ACCEPT, addr, prop, prop, cmd0, cmd1);
    }
}

// PaxosPluscal step P3
// Wait until either (1) the process is preempted by some ack message with
// higher proposal number, or (2) there is a majority of acceptors that sent ack
// messages, or (3) the number of received messages is the number of acceptors.
// Return OK or PREEMPTED.
int wor_write_receive (uint addr) {
    int count, ret;
    count = num_acceptors_get();
    ret = STATUS_OK;
    while (0 < count && ret == STATUS_OK && !wor_quorums_write_is_quorum(addr)) {
        ret = wor_handle_write_ack(addr); // ret = OK or PREEMPTED
        count--;
    }

    if (ret == STATUS_OK) { // last message received OK
        paxos_space_set_written_on(addr);
    }
    if (count == 0) {
        ret = STATUS_PREEMPTED;
    }
    return ret;
}
