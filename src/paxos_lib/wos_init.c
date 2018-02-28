#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "wor.h" // wor_read_cmd0_get
#include "wos_state.h"

// Initialize WORs in segment's body
void wos_body_init (uint seg) {
    int off;
    uint first_addr = SEG_FIRST(seg);
    off = 1;
    while (off < WOS_SIZE) {
        wor_init(first_addr + off);
        off++;
    }
}

// Return OK, ALREADYWRITTEN, TIMEOUT, EOS_ERROR
int wos_write (uint seg, uint addr, cmd_t cmd0, uint cmd1) {
    int res;
    // Check whether address to write is inside the segment's body.
    if (SEG_FIRST(seg) < addr && addr <= SEG_LAST(seg)) {
        res = wor_capture_and_write(addr, cmd0, cmd1);
        return res; // OK, ALREADYWRITTEN, TIMEOUT
    } else {
        return STATUS_EOS_ERROR;
    }
}

// Return the owner (client_id) of the segment, or NODE_ID_UNDEF when there is
// none or the segment is not allocated.
// If owner is known locally, return it. Otherwise, read it from WOS.
uint wos_owner (uint seg) {
    int res, owner;
    cmd_t cmd;
    owner = segment_owner(seg);
    if (owner == NODE_ID_UNDEF) {
        res = wor_read(SEG_FIRST(seg));
        if (res == STATUS_OK) {
            cmd = wor_read_cmd0_get();
            if (cmd == CMD_SEGMENT_OWNER) {
                owner = wor_read_cmd1_get();
                segment_owner_set(seg,owner);
                return owner;
            }
        }
        return NODE_ID_UNDEF;
    } else {
        return owner;
    }
}
