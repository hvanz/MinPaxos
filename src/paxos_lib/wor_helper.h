#ifndef _WOR_HELPER_H_
#define _WOR_HELPER_H_

#include "../paxos_common/data.h"

// Return OK, ALREADYWRITTEN, UNWRITTEN, ERROR
int wor_read_helper (uint addr);

#endif // _WOR_HELPER_H_
