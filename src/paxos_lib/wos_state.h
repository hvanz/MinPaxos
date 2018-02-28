#ifndef _WOS_STATE_H_
#define _WOS_STATE_H_

#include "../paxos_common/data.h"

#define WOS_SIZE 16
#define NUM_SEGMENTS (LOG_SIZE / WOS_SIZE)
// #define ADDR_IS_METADATA(addr) ((addr) % WOS_SIZE == 0)
// #define ADDR_METADATA(addr) ((addr) - ((addr) % WOS_SIZE)) // first WOR of the segment
#define SEG_FIRST(seg) ((seg) * WOS_SIZE) // log address of a segment's first entry
#define SEG_ADDR(seg,offset) (SEG_FIRST(seg) + offset)
#define SEG_LAST(seg) (SEG_FIRST(seg) + WOS_SIZE - 1)

uint segment_owner (uint seg);
uint segment_tail (uint seg);

void segment_owner_set (uint seg, uint owner_id);
void segment_tail_init (uint seg);
void segment_tail_inc (uint seg);

// int segment_valid_addr (uint seg, uint addr);

#endif // end of #ifndef _WOS_STATE_H_
