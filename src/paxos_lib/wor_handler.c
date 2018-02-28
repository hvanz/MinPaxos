#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "../paxos_common/messages.h"
#include "../paxos_common/comm.h"  // paxos_receive_msg
#include "../paxos_common/space.h"
#include "paxos_view.h" // paxos_socket_fd_get
#include "wor_quorums.h"

/******************************************************************************/

// The current proposal number old_prop is the initial proposal PROP_UNDEF or it
// was rejected by some acceptor. This function returns the next proposal
// number, or PROP_UNDEF when no more numbers are available.
uint wor_get_next_proposal (uint addr) {
    uint old_prop, new_prop, node_id;
    old_prop = paxos_space_get_prop(addr);
    node_id = node_id_get();
    new_prop = PROP_NEXT(old_prop, node_id);
    if (new_prop <= PROP_MAX) {
        paxos_space_set_prop(addr, new_prop);
    } else {
        new_prop = PROP_UNDEF;
    }
    return new_prop;
}

/******************************************************************************/

/* Receives one message and stores it in variable m.
 * Messages that are not for [addr] are ignored (and discarded).
 * Return OK or PREEMPTED.
 */
int wor_handle_capture_ack (uint addr) {
    msg_t m;
    int res;
    uint cur_prop, cur_vprop;
    res = receive_msg(paxos_socket_fd_get());
    if (res == STATUS_OK) {
        m.type = msg_type();
        m.from = msg_from();
        m.addr = msg_addr();
        m.prop = msg_prop();
        m.vprop = msg_vprop();
        m.vcmd0 = msg_vcmd0();
        m.vcmd1 = msg_vcmd1();
        if (m.type == MSG_CAPTURE_ACK &&
            m.addr == addr &&
            !wor_quorums_capture_mem(addr, m.from)) {
                cur_prop = paxos_space_get_prop(addr);
                cur_vprop = paxos_space_get_vprop(addr);
                if (cur_prop == m.prop) {
                    wor_quorums_capture_add(addr, m.from);
                    if (m.vprop != PROP_UNDEF &&
                        m.vcmd0 != CMD_UNDEF &&
                        cur_vprop < m.vprop) {
                        paxos_space_set_vprop(addr, m.vprop);
                        paxos_space_set_vcmd0(addr, m.vcmd0);
                        paxos_space_set_vcmd1(addr, m.vcmd1);
                    }
                    return STATUS_OK;
                } else if (cur_prop < m.prop) { // preempt
                    // The current proposal number cur_prop is small for some acceptor.
                    // Therefore we need to abort the current capture phase. When executing
                    // again capture_send, the proposal number will be increased.
                    return STATUS_PREEMPTED;
                } else {
                    return STATUS_OK; // ignore incoming message
                }
        } else {
            return STATUS_OK; // ignore incoming message
        }
    } else {
        return STATUS_OK; // ignore receive error
    }
}

int wor_handle_write_ack (uint addr) {
    msg_t m;
    int res;
    uint cur_prop;
    res = receive_msg(paxos_socket_fd_get());
    if (res == STATUS_OK) {
        m.type = msg_type();
        m.from = msg_from();
        m.addr = msg_addr();
        m.prop = msg_prop();
        m.vprop = msg_vprop();
        m.vcmd0 = msg_vcmd0();
        m.vcmd1 = msg_vcmd1();
        if (m.type == MSG_ACCEPT_ACK &&
            m.addr == addr &&
            !wor_quorums_write_mem(addr, m.from) &&
            m.vcmd0 != CMD_UNDEF) {
                cur_prop = paxos_space_get_prop(addr);
                if (cur_prop == m.prop) {
                    wor_quorums_write_add(addr, m.from);
                    paxos_space_set_vprop(addr, m.vprop);
                    paxos_space_set_vcmd0(addr, m.vcmd0);
                    paxos_space_set_vcmd1(addr, m.vcmd1);
                    return STATUS_OK;
                } else if (cur_prop < m.prop) { // preempt
                    return STATUS_PREEMPTED;
                } else {
                    return STATUS_OK; // ignore incoming message
                }
        } else {
            return STATUS_OK; // ignore incoming message
        }
    } else {
        return STATUS_OK;
    }
}
