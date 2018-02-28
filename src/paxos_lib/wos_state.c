#include "../paxos_common/paxos.h"
#include "../paxos_common/data.h"
#include "wos_state.h"

struct wos_state {
    uint owner_id;
    uint tail;
    // uint acceptors_view;
};

static struct wos_state wos_space[NUM_SEGMENTS];

uint segment_owner (uint seg) { return wos_space[seg].owner_id; }
uint segment_tail (uint seg) { return wos_space[seg].tail; }

void segment_owner_set (uint seg, uint owner_id) { wos_space[seg].owner_id = owner_id; }
void segment_tail_init (uint seg) { wos_space[seg].tail = SEG_FIRST(seg) + 1; }
void segment_tail_inc (uint seg) { if (wos_space[seg].tail < WOS_SIZE) wos_space[seg].tail++; }

// int segment_valid_addr (uint seg, uint addr) {
//     return (SEG_FIRST(seg) < addr && addr <= SEG_LAST(seg));
// }
