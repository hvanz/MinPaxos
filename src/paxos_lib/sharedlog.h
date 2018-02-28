#ifndef _SHAREDLOG_H_
#define _SHAREDLOG_H_

#include "../paxos_common/data.h"

/******************************************************************************/
/* Shared Log                                                                 */
/******************************************************************************/

void sharedlog_init (uint client_id);

uint sharedlog_append (cmd_t cmd0, uint cmd1);

int sharedlog_getnext (cmd_t * cmd_type, uint * cmd1);

// cmd_t sharedlog_getnext_cmd0 ();
// uint sharedlog_getnext_cmd0 ();
// uint sharedlog_getnext_seg ();
// uint sharedlog_getnext_off ();

int sharedlog_check ();

#endif /* _SHAREDLOG_H_ */
