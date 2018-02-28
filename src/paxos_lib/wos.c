#include <stdio.h>

#include "../paxos_common/paxos.h"
#include "wor.h"
#include "wos_state.h"
#include "wos_init.h" // wos_owner, wos_write

// wos_read return variables
cmd_t wos_read_cmd0;
uint wos_read_cmd1;

cmd_t wos_read_cmd0_get () { return wos_read_cmd0; }
uint wos_read_cmd1_get () { return wos_read_cmd1; }
void wos_read_cmd0_set (cmd_t x) { wos_read_cmd0 = x; }
void wos_read_cmd1_set (uint x) { wos_read_cmd1 = x; }

/******************************************************************************/

// Return OK, NOTOWNER or TIMEOUT
int wos_alloc (uint seg) {
    int res;
    uint node_id, owner_id, first_addr;
    node_id = node_id_get();
    first_addr = SEG_FIRST(seg);
    owner_id = wos_owner(seg);
    if (owner_id == NODE_ID_UNDEF) {
        res = wor_capture_and_write(first_addr, CMD_SEGMENT_OWNER, node_id);
        if (res == STATUS_OK) {
            wos_body_init(seg);
            segment_owner_set(seg, node_id);
            segment_tail_init(seg);
            // wos_space[seg].is_written = FALSE;
            return STATUS_OK;
        } else if (res == STATUS_ALREADYWRITTEN) {
            return STATUS_OK;
        } else { // res == TIMEOUT
            return STATUS_TIMEOUT;
        }
    } else {
        segment_owner_set(seg,owner_id);
        segment_tail_init(seg);
        return STATUS_NOTOWNER;
    }
}

// Return OK, EOS_ERROR, NOTOWNER, TIMEOUT
int wos_append (uint seg, cmd_t cmd0, uint cmd1) {
    uint owner_id, client_id, addr;
    int res;
    client_id = node_id_get();
    owner_id = wos_owner(seg);

    // Append if client owns the segment; if it does not, it can only write junk.
    if ((owner_id == client_id) ||
        (owner_id != client_id && cmd0 == CMD_JUNK)) {
        addr = SEG_ADDR(seg, segment_tail(seg));
        res = STATUS_ALREADYWRITTEN;
        while (res == STATUS_ALREADYWRITTEN) {
            res = wos_write(seg, addr, cmd0, cmd1);
            if (res == STATUS_OK || res == STATUS_ALREADYWRITTEN) {
                segment_tail_inc(seg);
                addr++;
            }
        }
        return res; // res = OK, EOS_ERROR or TIMEOUT
    } else {
        return STATUS_NOTOWNER;
    }
}

// Return OK, UNWRITTEN, EOS_ERROR, TIMEOUT
// If OK, set return variable to (cmd,arg1); otherwise, set it to (CMD_UNDEF,0).
int wos_read (uint seg, uint offset) {
    uint addr;
    int res;
    wos_read_cmd0_set(CMD_UNDEF);
    // wos_read_cmd0_set(0);
    addr = SEG_ADDR(seg, offset);
    if (SEG_FIRST(seg) < addr && addr <= SEG_LAST(seg)) {
        res = wor_read(addr);
        if (res == STATUS_OK) {
            wos_read_cmd0_set(wor_read_cmd0_get());
            wos_read_cmd1_set(wor_read_cmd1_get());
        }
        return res; // OK, UNWRITTEN or TIMEOUT
    } else {
        return STATUS_EOS_ERROR;
    }
}

// Checks whether the WOS has been initialized (i.e., if it has an owner)
int wos_test (uint seg) {
    uint owner_id;
    owner_id = wos_owner(seg);
    return (owner_id != NODE_ID_UNDEF);
}
