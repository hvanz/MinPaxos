#ifndef _WOR_HANDLER_H_
#define _WOR_HANDLER_H_

// Return the next proposal number.
uint wor_get_next_proposal (uint addr);

// Return OK or PREEMPTED.
int wor_handle_capture_ack (uint addr);
int wor_handle_write_ack (uint addr);

#endif // _WOR_HANDLER_H_
