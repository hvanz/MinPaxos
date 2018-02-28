#ifndef _WOS_H_
#define _WOS_H_

#include "../paxos_common/data.h"

/******************************************************************************/
/* WOS API                                                                    */
/******************************************************************************/

// Return STATUS_ALREADYWRITTEN when the segment is owned by someone else.
// Return OK, ALREADYWRITTEN or TIMEOUT
int wos_alloc (uint seg);

// Checks whether the WOS has been initialized (i.e., if it has an owner)
int wos_test (uint seg);

// Append command [cmd0,cmd1] to next available address in segment [seg].
// Return OK, EOS_ERROR, NOTOWNER, TIMEOUT
int wos_append (uint seg, cmd_t cmd0, uint cmd1);

// Return OK, UNWRITTEN, EOS_ERROR, TIMEOUT
// If OK, set return variable to (cmd,arg1); otherwise, set it to (CMD_UNDEF,0).
int wos_read (uint seg, uint offset);

// Getters for wos_read() return values
cmd_t wos_read_cmd0_get ();
uint wos_read_cmd1_get ();

#endif // end of #ifndef _WOS_H_
