#ifndef _WOS_INIT_H_
#define _WOS_INIT_H_

#include "../paxos_common/data.h"
#include "wos_state.h"

// Return the owner (client_id) of the segment, or NODE_ID_UNDEF when there is
// none or the segment is not allocated.
uint wos_owner (uint seg);

// Initialize WORs in segment's body
void wos_body_init (uint seg);

// Append command [cmd0,cmd1] to address [addr] in segment [seg].
// Return OK, EOS_ERROR, TIMEOUT
int wos_write (uint seg, uint addr, cmd_t cmd0, uint cmd1);

#endif // end of #ifndef _WOS_INIT_H_
