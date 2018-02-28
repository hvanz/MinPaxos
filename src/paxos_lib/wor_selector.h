#ifndef _WOR_SELECTOR_H_
#define _WOR_SELECTOR_H_

#include "../paxos_common/data.h"

int wor_selected_value_is_undef (uint addr);
cmd_t wor_selected_cmd0_get ();
uint wor_selected_cmd1_get ();
int wor_select_value (uint addr, cmd_t cmd0, uint cmd1);

#endif // _WOR_SELECTOR_H_
