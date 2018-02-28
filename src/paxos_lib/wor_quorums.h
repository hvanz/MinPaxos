#ifndef _WOR_QUORUMS_H_
#define _WOR_QUORUMS_H_

void wor_quorums_capture_reset (uint addr);
void wor_quorums_capture_add (uint addr, uint acceptor_id);
uint wor_quorums_capture_is_quorum (uint addr);
int wor_quorums_capture_mem (uint addr, uint acceptor_id);

void wor_quorums_write_reset (uint addr);
void wor_quorums_write_add (uint addr, uint acceptor_id);
uint wor_quorums_write_is_quorum (uint addr);
int wor_quorums_write_mem (uint addr, uint acceptor_id);

#endif // _WOR_QUORUMS_H_
