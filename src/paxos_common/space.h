#ifndef _PAXOS_SPACE_H_
#define _PAXOS_SPACE_H_

#include "data.h"

void paxos_space_init (uint addr);
void paxos_space_init_all ();

uint paxos_space_get_prop (uint addr);
uint paxos_space_get_vprop (uint addr);
cmd_t paxos_space_get_vcmd0 (uint addr);
uint paxos_space_get_vcmd1 (uint addr);
uint paxos_space_is_written (uint addr);

void paxos_space_set_prop  (uint addr, uint x);
void paxos_space_set_vprop (uint addr, uint x);
void paxos_space_set_vcmd0 (uint addr, cmd_t x);
void paxos_space_set_vcmd1 (uint addr, uint x);
void paxos_space_set_written_on (uint addr);
void paxos_space_set_written_off (uint addr);

#endif /* _PAXOS_SPACE_H_ */
