#ifndef _COMM_UDP_H_
#define _COMM_UDP_H_

#include <arpa/inet.h>
#include "messages.h"

void paxos_socket_fd_set (int sockfd);
int paxos_socket_fd_get ();

// Socket error codes
#define SOCKET_SUCCESS			0
#define SOCKET_INIT_FAIL		-1
#define SOCKET_ACCEPT_FAIL		-2
#define SOCKET_RECV_FAIL		-3
#define SOCKET_SEND_FAIL		-4

#define SOCKET_FD_UNDEF     0

int comm_init (uint addr, uint port);

// int send_msg (int sockfd, char * buffer, int size, uint dest_addr, uint dest_port);
void send_msg (int sockfd, uint dest_addr, uint dest_port,
               msg_type_t type, uint from, uint addr, uint max,
               uint vnum, cmd_t vcmd0, uint vcmd1);

int receive_msg (int sockfd);

// If receive_msg() returns OK, then the message and the sender address and port
// can be obtained with the following functions.
msg_type_t msg_type ();
uint msg_addr ();
uint msg_prop ();
uint msg_from ();
uint msg_vprop ();
cmd_t msg_vcmd0 ();
uint msg_vcmd1 ();
uint peer_addr ();
uint peer_port ();

#endif /* _COMM_UDP_H_ */
