#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // ntohs
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "data.h"
#include "messages.h"
#include "comm.h"

static int paxos_socket_fd; // unique receiver/sender

void paxos_socket_fd_set (int sockfd) { paxos_socket_fd = sockfd; }
int paxos_socket_fd_get () { return paxos_socket_fd; }

/******************************************************************************/

// Return socket file descriptor bound to local address nbo_addr:nbo_port
// nbo = network byte order
int comm_init (uint nbo_addr, uint nbo_port) {
    int sockfd, res;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return SOCKET_FD_UNDEF; }

    addr.sin_family = AF_INET;
    addr.sin_port = nbo_port;
    addr.sin_addr.s_addr = nbo_addr;
    res = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (res < 0) { perror("bind"); return SOCKET_FD_UNDEF; }

    printf("# Created socket bound to %s:%u\n",inet_ntoa((struct in_addr) { .s_addr = nbo_addr }), ntohs(nbo_port));
    // printf("# %s:%u -> %u:%u\n",hostname,ACCEPTORS_PORT,inet_addr(hostname),HTONS_ACCEPTORS_PORT(0));
    return sockfd;
}

/******************************************************************************/

int comm_send (int sockfd, char * buffer, int size, uint dest_addr, uint dest_port) {
    struct sockaddr_in remote_addr;
    int n;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = dest_port;
    remote_addr.sin_addr.s_addr = dest_addr;
    n = sendto(sockfd, (const void *) buffer, size, 0,
            (struct sockaddr *) &remote_addr, sizeof(remote_addr));
    if (n != size) { perror("sendto"); return SOCKET_SEND_FAIL; }

    printf("[n%d] > Sent to %s:%u:\t -> ", node_id_get(), inet_ntoa((struct in_addr) { .s_addr = dest_addr }), ntohs(dest_port));
    print_msg((msg_t *) buffer);
    return STATUS_OK;
}


void send_msg (int sockfd, uint dest_addr, uint dest_port,
               msg_type_t type, uint from, uint addr, uint prop,
               uint vnum, cmd_t vcmd0, uint vcmd1) {
    if (sockfd == 0) return; //STATUS_ERROR;
    msg_t m;
    int res;
    m.type = type;
    m.from = from;
    m.addr = addr;
    m.prop = prop;
    m.vprop = vnum;
    m.vcmd0 = vcmd0;
    m.vcmd1 = vcmd1;
    res = comm_send(sockfd, (char *) &m, sizeof(msg_t), dest_addr, dest_port);
    if (res != STATUS_OK) {
        exit(1); //EXIT_FAILURE
    }
}

/******************************************************************************/

static msg_t paxos_recvd_msg; // last received message

msg_type_t msg_type () { return paxos_recvd_msg.type; }
uint msg_addr () { return paxos_recvd_msg.addr; }
uint msg_prop () { return paxos_recvd_msg.prop; }
uint msg_from () { return paxos_recvd_msg.from; }
uint msg_vprop () { return paxos_recvd_msg.vprop; }
cmd_t msg_vcmd0 () { return paxos_recvd_msg.vcmd0; }
uint msg_vcmd1 () { return paxos_recvd_msg.vcmd1; }

static uint paxos_recvd_peer_addr; // remote address of the last received message
static uint paxos_recvd_peer_port; // remote port of the last received message; in network byte order

uint peer_addr () { return paxos_recvd_peer_addr; }
uint peer_port () { return paxos_recvd_peer_port; }

// Set up the (static) variables last_msg, last_peer_addr, last_peer_port.
// The message received can be accesed with the functions msg_[field](), one
// field at a time.
// Return STATUS_OK or SOCKET_RECV_FAIL.
// If OK, fetches an incoming message and stores it in paxos_recvd_msg, and
// sender address and port in last_peer_addr and last_peer_port.
int receive_msg (int sockfd) {
    if (sockfd == 0) return STATUS_ERROR;
    struct sockaddr_in peer_addr;
    uint peer_addr_len = sizeof(peer_addr); //it has to contain the expected address size
    int n;
    n = recvfrom(sockfd, (char *) &paxos_recvd_msg, sizeof(msg_t), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
    if (n < 0) { perror("recvfrom"); return SOCKET_RECV_FAIL; }
    else {
        paxos_recvd_peer_addr = peer_addr.sin_addr.s_addr;
        paxos_recvd_peer_port = peer_addr.sin_port;
        printf("[n%d] < Received from %s:%u:\t <- ", node_id_get(), inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
        print_msg(&paxos_recvd_msg);
        return STATUS_OK;
    }
}
