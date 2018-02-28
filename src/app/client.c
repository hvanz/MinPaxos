#include <stdio.h>
#include <stdlib.h>

#include "../paxos_common/data.h"
#include "repcounter.h"

int main(int argc, char *argv[]) {
    int client_id;
    if (argc < 2) {
      fprintf(stderr, "Usage: %s <client-id>\n", argv[0]);
      exit(1);
    }
    client_id = atoi(argv[1]);

    /***************************************************************************/
    int x;

    repcounter_init(client_id);

    x = repcounter_read();
    printf("counter value = %d\n", x);

    repcounter_inc(10);
    x = repcounter_read();
    printf("counter value = %d\n", x);

    repcounter_destroy();

    return 0;
}
