#include <stdio.h>
#include <stdlib.h>

#include "paxos.h"
#include "data.h"

void print_cmd0 (cmd_t cmd0) {
    switch (cmd0) {
    case CMD_UNDEF:           printf("UNDEF"); break;
    case CMD_SEGMENT_OWNER:   printf("SEGMENT_OWNER"); break;
    case CMD_JUNK:            printf("JUNK"); break;
    case CMD_RECONFIGURE:     printf("RECONFIGURE"); break;
    case CMD_REPCOUNTER_INC:  printf("REPCOUNTER_INC"); break;
    case CMD_REPCOUNTER_READ: printf("REPCOUNTER_READ"); break;
    default:;
    }
}
void print_cmd (cmd_t cmd0, uint cmd1) {
    printf("(");
    print_cmd0(cmd0);
    printf(",%u)", cmd1);
}

uint node_id;

uint node_id_get (void) { return node_id; }
void node_id_set (uint x) { node_id = x; }
