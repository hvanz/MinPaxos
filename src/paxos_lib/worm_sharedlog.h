#ifndef _WORM_SHAREDLOG_H_
#define _WORM_SHAREDLOG_H_

#include "../paxos_common/data.h"

/******************************************************************************/
/* WormSpace                                                                  */
/******************************************************************************/

void worm_sharedlog_init (uint client_id);

uint worm_sharedlog_append (cmd_t cmd0, uint cmd1);

// fetches next unexecuted command
int worm_sharedlog_getnext ();
cmd_t worm_sharedlog_getnext_cmd0 ();
uint worm_sharedlog_getnext_cmd1 ();

// uint sharedlog_getnext_seg ();
// uint sharedlog_getnext_off ();

// int sharedlog_check ();

#endif /* _WORM_SHAREDLOG_H_ */
