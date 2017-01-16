#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "btclient.h"

#define SERV_ADDR   "70:56:81:92:89:AA"     /* TO BE DEFINED: (Whatever the address of the server is) */
#define TEAM_ID     5                       /* Napoleon ID */

#define MSG_ACK     0
#define MSG_NEXT    1
#define MSG_START   2
#define MSG_STOP    3
#define MSG_CUSTOM  4
#define MSG_KICK    5
#define MSG_POSITION 6
#define MSG_BALL    7

//void debug (const char *fmt, ...) {
//    va_list argp;
//
//    va_start (argp, fmt);
//
//    vprintf (fmt, argp);
//
//    va_end (argp);
//}


//TO BE DECLARED INTO THE MAIN FILE
//uint8_t role = 0x00;      /* 0 -> Beginner; 1 -> Finisher */
//uint8_t side = 0x00;      /* 0 -> Right; 1 -> Left */
//uint8_t ally = 0x00;      /* ID of the robot in the same team */
//uint16_t msgId = 0x0000;
//int s;

/* Read data from the server */
int read_from_server (int sock, char *buffer, size_t maxSize) {
    int bytes_read = read (sock, buffer, maxSize);

/*    if (bytes_read <= 0) {
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        close (sock);
        exit (EXIT_FAILURE);
    }
*/

    printf ("[DEBUG] received %d bytes\n", bytes_read);

    return bytes_read;
}

/* Initialize bluetooth connection */
int bt_init(int *s){
    struct sockaddr_rc addr = { 0 };
    int status;
    
    /* Allocate a socket */
    *s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    /* Set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (SERV_ADDR, &addr.rc_bdaddr);
    
    /* Connect to server */
    status = connect(*s, (struct sockaddr *)&addr, sizeof(addr));
    
    /* Return 0 if connected */
    return status;
}

int bt_close(int *s){
    return close (*s);
}

/* Read and check */
int bt_check(int s){
    ssize_t nbyte;
    char string[58]; 

    /* Read from server */
    nbyte = read_from_server (s, string, 7);

    if(nbyte==-1){
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        bt_close (&s);
        exit (EXIT_FAILURE);
    }
    else if(nbyte==0){
	printf ("Nothing to read\n");
        return 0;
    }
    else{
	switch(string[4]) {
	    case MSG_ACK :
		break;
	    case MSG_NEXT :
		break;
	    case MSG_START :
		break;
	    case MSG_STOP :
		break;
	    case MSG_KICK :
		break;
	    case MSG_BALL :
		break;
	    default :
		printf("Invalid msg\n");
		break;
	}
   }

}

/* Send an ACK message */
ssize_t bt_send_ack(int s, uint16_t msgId, uint16_t ackId, uint8_t dest, uint8_t statusCode){
    char string[58];
    
    // Remember to increment msgId
    *((uint16_t *) string) = msgId;
    string[2] = TEAM_ID;
    string[3] = dest;
    string[4] = MSG_ACK;
    *((uint16_t *) string+5) = ackId;
    string[7] = statusCode;
    
    /* Return number of bytes written */
    return write(s, string, 8);
}

/* Send a NEXT message to the ally */
ssize_t bt_send_next(int s, uint16_t msgId, uint8_t ally){
    char string[58];
    
    // Remember to increment msgId
    *((uint16_t *) string) = msgId;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_NEXT;
    
    /* Return number of bytes written */
    return write(s, string, 5);
}

/* Receive a START message from the server */
ssize_t bt_start(int s, uint8_t *role, uint8_t *side, uint8_t *ally){
    char string[58];
    ssize_t nbyte;
    
    /* Read from server */
    nbyte = read_from_server (s, string, 7);
    
    if (string[4] == MSG_START) {
        printf ("Received start message!\n");
        *role = (uint8_t) string[5];
        *side = (uint8_t) string[6];
        *ally = (uint8_t) string[7];
    }
    else
        nbyte = -1;
    
    /* Return number of bytes read */
    return nbyte;
}

/* Receive a STOP message from the server */
ssize_t bt_stop(int s){
    char string[58];
    ssize_t nbyte;
    
    /* Read from server */
    nbyte = read_from_server (s, string, 5);
    
    if (string[4] == MSG_STOP) {
        printf ("Received stop message!\n");
    }
    else
        nbyte = -1;
    
    /* Return number of bytes read */
    return nbyte;
}

/* Receive a KICK message from the server */
ssize_t bt_kick(int s){
    char string[58];
    ssize_t nbyte;
    
    /* Read from server */
    nbyte = read_from_server (s, string, 6);
    
    if (string[4] == MSG_KICK) {
        printf ("Received stop message!\n");
    }
    else
        nbyte = -1;
    
    /* Return number of bytes read */
    return nbyte;
}

/* Send a POSITION message to the server */
ssize_t bt_send_position(int s, uint16_t msgId, int16_t x, int16_t y){
    char string[58];
    
    // Remember to increment msgId
    *((uint16_t *) string) = msgId;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    // Little endian representation
    // TO BE CHECKED
    string[5] = (uint8_t)(x);
    string[6] = (uint8_t)(x>>8);
    string[7] = (uint8_t)(y);
    string[8]= (uint8_t)(y>>8);
    
    /* Return number of bytes written */
    return write(s, string, 9);
}

/* Send a BALL message to the ally */
ssize_t bt_send_ball(int s, uint16_t msgId, uint8_t ally, uint8_t pick_notDrop, int16_t x, int16_t y){
    char string[58];
    
    // Remember to increment msgId
    *((uint16_t *) string) = msgId;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_BALL;
    string[5] = pick_notDrop;
    // Little endian representation
    // TO BE CHECKED
    string[6] = (uint8_t)(x);
    string[7] = (uint8_t)(x>>8);
    string[8] = (uint8_t)(y);
    string[9]= (uint8_t)(y>>8);
    
    /* Return number of bytes written */
    return write(s, string, 10);
}

//int main(int argc, char **argv) {
//    struct sockaddr_rc addr = { 0 };
//    int status;
//
//    /* allocate a socket */
//    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
//
//    /* set the connection parameters (who to connect to) */
//    addr.rc_family = AF_BLUETOOTH;
//    addr.rc_channel = (uint8_t) 1;
//    str2ba (SERV_ADDR, &addr.rc_bdaddr);
//
//    /* connect to server */
//    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
//
//    /* if connected */
//    if( status == 0 ) {
//        char string[58];
//
//        /* Wait for START message */
//        read_from_server (s, string, 9);
//        if (string[4] == MSG_START) {
//            printf ("Received start message!\n");
//            rank = (unsigned char) string[5];
//            next = (unsigned char) string[7];
//        }
//
//        if (rank == 0)
//            beginner ();
//        else
//            finisher ();
//
//        close (s);
//
//        sleep (5);
//
//    } else {
//        fprintf (stderr, "Failed to connect to server...\n");
//        sleep (2);
//        exit (EXIT_FAILURE);
//    }
//
//    close(s);
//    return 0;
//}


