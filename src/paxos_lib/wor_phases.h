#ifndef _WOR_CAPTURE_H_
#define _WOR_CAPTURE_H_

// Retries MAX_RETRIES times to capture.
// Return OK, ALREADYWRITTEN, TIMEOUT, ERROR.
int wor_capture (uint addr);

// Return OK, PREEMPTED.
int wor_write (uint addr, cmd_t cmd0, uint cmd1);

#endif // _WOR_CAPTURE_H_
