#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "btclient.h"

int main(int argc, char **argv) {

    uint8_t role = 0x00;      /* 0 -> Beginner; 1 -> Finisher */
    uint8_t side = 0x00;      /* 0 -> Right; 1 -> Left */
    uint8_t ally = 0x00;      /* ID of the robot in the same team */
    uint16_t msgId = 0x0000;
    int s;
    char buffer[58];

    /* if connected */
    if( bt_init(&s) == 0 ) {
        printf("Connected!\n");

	//TO BE REMOVED IN THE COMPETITION
        bt_kick(s);
        bt_kick(s);
        bt_kick(s);
	
        /* Wait for START message */
        bt_start(s, &role, &side, &ally);
	printf("Role: %d, Side: %d, Ally: %d\n", role, side, ally);
        if (role == 0)
	    printf("Beginner\n");
            //beginner ();
        else
	    printf("Finisher\n");
            //finisher ();

    } else {
        fprintf (stderr, "Failed to connect to server...\n");
        sleep (2);
        exit (EXIT_FAILURE);
    }

    bt_close(&s);
    return 0;
}
