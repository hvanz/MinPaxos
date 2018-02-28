#include <stdio.h>
#include <stdlib.h> //exit, atoi
#include <sys/types.h>

#include "../paxos_common/messages.h"
#include "../paxos_common/space.h"
#include "../paxos_common/comm.h"

void acceptor_msg_handler () {
    int sockfd, res;
    uint dst_addr, dst_port, node_id, prop, vprop, vcmd0, vcmd1;
    uint m_type, m_prop, m_addr, m_vprop, m_vcmd0, m_vcmd1;
    //msg_t m;
    node_id = node_id_get();
    if (node_id != 0) {
        sockfd = paxos_socket_fd_get();
        if (0 < sockfd) {
            res = receive_msg(sockfd); // set up recvd_msg, peer_addr, peer_port
            if (res == STATUS_OK) {
                m_type = msg_type();
                // m_from = msg_from();
                m_prop = msg_prop();
                m_addr = msg_addr();
                m_vprop = msg_vprop();
                m_vcmd0 = msg_vcmd0();
                m_vcmd1 = msg_vcmd1();
                dst_addr = peer_addr();
                dst_port = peer_port();
                if (/*0 <= m_addr &&*/ m_addr <= LOG_SIZE) {
                    prop = paxos_space_get_prop(m_addr);
                    vprop = paxos_space_get_vprop(m_addr);
                    vcmd0 = paxos_space_get_vcmd0(m_addr);
                    vcmd1 = paxos_space_get_vcmd1(m_addr);

                    if (m_type == MSG_CAPTURE) { /* Phase 1b */
                        if (prop < m_prop) {
                            paxos_space_set_prop(m_addr, m_prop);
                        }
                        send_msg (sockfd, dst_addr, dst_port,
                                  MSG_CAPTURE_ACK, node_id, m_addr, prop, vprop, vcmd0, vcmd1);
                	} else if (m_type == MSG_ACCEPT) { /* Phase 2b */
                	    if (m_prop != 0 && //PAXOS_PROP_UNDEF &&
                            m_vcmd0 != 0 && //PAXOS_CMD_UNDEF &&
                            prop <= m_prop) {
                            paxos_space_set_prop(m_addr, m_prop);
                            paxos_space_set_vprop(m_addr, m_prop);
                            paxos_space_set_vcmd0(m_addr, m_vcmd0);
                            paxos_space_set_vcmd1(m_addr, m_vcmd1);
                            send_msg (sockfd, dst_addr, dst_port,
                                      MSG_ACCEPT_ACK, node_id, m_addr, m_prop, m_vprop, m_vcmd0, m_vcmd1);
                	    }
                    } //else { perror("unknown message type\n"); }
                }
            }
        }
    }
}

// while(1) {...} cannot be specified; acc_while_one stays unverified
void acceptor_while_one () {
    while (1) {
        acceptor_msg_handler();
    }
}

void acceptor_init (uint acc_id, uint port) {
    int sockfd;
    node_id_set(acc_id);
    paxos_space_init_all();
    sockfd = comm_init(INET_ADDR_NET, port);
    paxos_socket_fd_set(sockfd);
    // if (sockfd != 0) {
    //     paxos_socket_fd_set(sockfd);
    // }
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <acceptor-id>\n", argv[0]);
        return 1;
    }
    int acc_id;
    acc_id = atoi(argv[1]);

    acceptor_init(acc_id, htons(ACCEPTORS_PORT + acc_id));
    acceptor_while_one();
    return 0; // non reachable
}
