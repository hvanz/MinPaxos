#ifndef _WOR_STEPS_H_
#define _WOR_STEPS_H_

#include "../paxos_common/data.h"

// Performs capture phase (Paxos phase 1) on log entry [addr].
// Takes proposal number from the log. This number is either a number set by a
// previous preemption, or the initial number, which can be either used as it is
// or rejected by some acceptor (and then the phase preempted).
// Received messages are processed by paxos_receiver_thread ().
// Returns STATUS_OK, if log[addr] is ready for next phase (log[addr].capture = TRUE)
// Returns STATUS_PREEMPTED, if some acceptor returned a proposal number greater
//   than current local number.

// Return OK, ALREADYWRITTEN, ERROR.
int wor_capture_send (uint addr);
// Return OK or PREEMPTED.
int wor_capture_receive (uint addr);

// Performs accept phase (Paxos phase 2) on log entry [addr].
// It assumes log[addr] is captured.
// Returns STATUS_OK when the write has succeeded: all future reads will return
//   the value written to it.
// Returns STATUS_ALREADYWRITTEN when the WOR is already written by someone else.
// Returns STATUS_PREEMPTED when some acceptor answered with a higher proposal.

void wor_write_send (uint addr, cmd_t cmd0, uint cmd1);
// Return OK or PREEMPTED.
int wor_write_receive (uint addr);

#endif // _WOR_STEPS_H_
