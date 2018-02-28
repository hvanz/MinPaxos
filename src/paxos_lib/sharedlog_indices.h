#ifndef _SHAREDLOG_INDICES_H_
#define _SHAREDLOG_INDICES_H_

#include "../paxos_common/data.h"

/******************************************************************************/
/* Shared Log Indices                                                         */
/******************************************************************************/

/* Prefixes invariant:
 * 0 <= first_unexecuted_addr < first_unwritten_addr <= first_blank_addr < LOG_SIZE
 */

uint first_unexecuted_addr_get ();
uint first_unwritten_addr_get ();
uint fetch_and_inc_first_blank_addr ();
void sharedlog_indices_init ();
void update_first_unwritten_addr ();
void increment_first_unexecuted_addr ();

#endif /* _SHAREDLOG_INDICES_H_ */
