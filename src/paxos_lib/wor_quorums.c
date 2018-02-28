#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "paxos_view.h"

uint wor_quorums_capture[LOG_SIZE][MAX_ACCEPTORS];
uint wor_quorums_write[LOG_SIZE][MAX_ACCEPTORS];

void wor_quorums_capture_reset (uint addr) {
    int i = num_acceptors_get() - 1;
    while (0 <= i) {
        wor_quorums_capture[addr][i] = 0;
        i--;
    }
}

void wor_quorums_capture_add (uint addr, uint acceptor_id) {
    wor_quorums_capture[addr][acceptor_id] = 1;
}

uint wor_quorums_capture_is_quorum (uint addr) {
    int i, count, nacc;
    nacc = num_acceptors_get();
    count = 0;
    i = nacc - 1;
    while (0 <= i) {
        if (wor_quorums_capture[addr][i] == 1) {
            count++;
        }
        i--;
    }
    return (count * 2 > nacc);
}

int wor_quorums_capture_mem (uint addr, uint acceptor_id) {
    if (wor_quorums_capture[addr][acceptor_id] == 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void wor_quorums_write_reset (uint addr) {
    int i = num_acceptors_get() - 1;
    while (0 <= i) {
        wor_quorums_write[addr][i] = 0;
        i--;
    }
}

void wor_quorums_write_add (uint addr, uint acceptor_id) {
    wor_quorums_write[addr][acceptor_id] = 1;
}

uint wor_quorums_write_is_quorum (uint addr) {
    int i, count, nacc;
    nacc = num_acceptors_get();
    count = 0;
    i = nacc - 1;
    while (0 <= i) {
        if (wor_quorums_write[addr][i] == 1) {
            count++;
        }
        i--;
    }
    return (count * 2 > nacc);
}

int wor_quorums_write_mem (uint addr, uint acceptor_id) {
    if (wor_quorums_write[addr][acceptor_id] == 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}
