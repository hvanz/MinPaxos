#include <stdio.h>
#include <string.h>

#include "messages.h"

void print_msg_type (msg_type_t t) {
    switch (t) {
    case MSG_CAPTURE:     printf("MSG_CAPTURE"); break;
    case MSG_CAPTURE_ACK: printf("MSG_CAPTURE_ACK"); break;
    case MSG_ACCEPT:      printf("MSG_ACCEPT"); break;
    case MSG_ACCEPT_ACK:  printf("MSG_ACCEPT_ACK"); break;
    default: printf("Unknown message type %d.\n", t);
    }
}

void print_msg (msg_t * m) {
    print_msg_type(m->type);
    printf(" ");
    printf("from: %u, ", m->from);
    printf("addr: %u, ", m->addr);
    printf("prop: %u, ", m->prop);
    printf("vote: [%u, (%u,%u)], ", m->vprop, m->vcmd0, m->vcmd1);
    // printf("addr_last: %u, ", m->addr_last_committed);
    // printf("addr_next: %u", m->addr_next_empty);
    printf("\n");
}
