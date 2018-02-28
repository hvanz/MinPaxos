#ifndef _WOR_H_
#define _WOR_H_

#include "../paxos_common/data.h"

/******************************************************************************/
/* WOR API                                                                    */
/******************************************************************************/

void wor_init (uint addr);

// Retries MAX_RETRIES times to capture.
// Return OK, ALREADYWRITTEN, TIMEOUT, ERROR.
int wor_capture (uint addr); // see wor_phases.h

// NB: It assumes log[addr] is captured.
// Return OK, ALREADYWRITTEN or TIMEOUT.
int wor_write (uint addr, cmd_t cmd0, uint cmd1);

// Try to reach consensus on a command for some log address.
// Orchestrates a capture phase followed by an accept phase.
// [count] is the number of times the function can be retried.
// If any of both phases is preempted, retry starting from the beginning (phase 1).
// Each preemption increases the proposal number, so the following capture phase
// won't be repeated.
// If succesful, set log[addr].is_written = TRUE
// Returns STATUS_OK if the read succeeded.
// Returns STATUS_ALREADYWRITTEN when the WOR is already written by someone else.
// Returns STATUS_TIMEOUT when MAX_RETRIES reached.
// Return OK, ALREADYWRITTEN, TIMEOUT, ERROR
int wor_capture_and_write (uint addr, cmd_t cmd0, uint cmd1);

// Performs capture phase followed by accept phase, with retries.
// Returns STATUS_OK if the read succeeded.
// Returns STATUS_UNWRITTEN if the address is unwritten.
// Returns STATUS_TIMEOUT when MAX_READ_RETRIES reached
// Return OK, ALREADYWRITTEN, UNWRITTEN, TIMEOUT, ERROR
// If OK, set return variable to (cmd,arg1); otherwise, set it to (CMD_UNDEF,0).
int wor_read (uint addr);

// Use these functions to obtain the result of wor_read() when it returns OK.
cmd_t wor_read_cmd0_get();
uint wor_read_cmd1_get();

#endif // _WOR_H_
