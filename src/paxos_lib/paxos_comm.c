#include <stdio.h>
#include <unistd.h> //close

#include "../paxos_common/messages.h"
#include "../paxos_common/comm.h"
#include "paxos_view.h"
#include "paxos_comm.h"

int paxos_lib_comm_init (uint client_id) {
    int sockfd;
//    sockfd = comm_init(htons(INADDR_ANY), htons(CLIENTS_PORT_BASE + client_id));
    sockfd = comm_init(INET_ADDR_NET, htons(CLIENTS_PORT_BASE + client_id));
    if (sockfd == 0) {
        return STATUS_ERROR;
    } else {
        paxos_socket_fd_set(sockfd);
        return STATUS_OK;
    }
}

// not used
// void paxos_lib_comm_close () {
//     close(paxos_socket_fd_get());
// }

// Build message and broadcast it to all acceptors.
void paxos_broadcast_msg (msg_type_t type, uint addr, uint prop,
                          uint vprop, cmd_t vote_cmd0, uint vote_cmd1) {
    int sockfd, acc_id;
    uint acc_port, acc_addr, node_id;

    sockfd = paxos_socket_fd_get();
    node_id = node_id_get();
    acc_id = num_acceptors_get() - 1;

    // if (sockfd != 0) {
    while (0 <= acc_id) {
        acc_addr = client_view_addr(acc_id);
        acc_port = client_view_port(acc_id);
        send_msg(sockfd, acc_addr, acc_port, type, node_id, addr, prop,
                 vprop, vote_cmd0, vote_cmd1);
        acc_id--;
    }
    // }
}
