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

//TO BE DECLARED INTO THE MAIN FILE
//uint8_t role = 0x00;      /* 0 -> Beginner; 1 -> Finisher */
//uint8_t side = 0x00;      /* 0 -> Right; 1 -> Left */
//uint8_t ally = 0x00;      /* ID of the robot in the same team */
uint16_t msgId = 0x0000;
int s;

/* Initialize bluetooth connection */
int bt_init(){
    struct sockaddr_rc addr = { 0 };
    int status;

    /* Allocate a socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* Set the connection parameters (who to connect to) */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (SERV_ADDR, &addr.rc_bdaddr);

    /* Connect to server */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    /* Return 0 if connected */
    return status;
}

int bt_close(){
    return close (s);
}

/* Read and check */
int bt_check(){
    ssize_t nbyte;
    char msg[58]; 
    

    /* Read from server */
    //nbyte = read_from_server (s, string, 7);
    nbyte = read (s, msg, 7);

    if(nbyte==-1){
        fprintf (stderr, "Server unexpectedly closed connection...\n");
        bt_close (s);
        exit (EXIT_FAILURE);
    }
    else if(nbyte==0){
        printf ("BT: Nothing to read\n");
        return 0;
    }
    else{
        switch(string[4]) {
            case MSG_ACK :
                bt_recv_ack(msg, nbyte);
                break;
            case MSG_NEXT :
                bt_recv_next(msg, nbyte);
                break;
            case MSG_START :
                bt_recv_start(msg, nbyte);
                break;
            case MSG_STOP :
                bt_recv_stop(msg, nbyte);
                break;
            case MSG_KICK :
                bt_recv_stop(msg, nbyte);
                break;
            case MSG_BALL :
                bt_recv_ball(msg, nbyte);
                break;
            default :
                printf("BT: Invalid message\n");
                break;
        }
    }
}


/*******************************  SENDING ******************************************/


/* Send an ACK message */
ssize_t bt_send_ack(uint16_t ackId, uint8_t dest, uint8_t statusCode){
    char string[58];

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = dest;
    string[4] = MSG_ACK;
    *((uint16_t *) string+5) = ackId;
    string[7] = statusCode;

    /* Return number of bytes written */
    return write(s, string, 8);
}

/* Send a NEXT message to the ally */
ssize_t bt_send_next(uint8_t ally){
    char string[58];

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_NEXT;

    /* Return number of bytes written */
    return write(s, string, 5);
}


/* Send a POSITION message to the server */
ssize_t bt_send_position(int16_t x, int16_t y){
    char string[58];

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    // Little endian representation
    string[5] = (uint8_t)(x);
    string[6] = (uint8_t)(x>>8);
    string[7] = (uint8_t)(y);
    string[8]= (uint8_t)(y>>8);

    /* Return number of bytes written */
    return write(s, string, 9);
}

/* Send a BALL message to the ally */
ssize_t bt_send_ball(uint8_t ally, uint8_t pick_notDrop, int16_t x, int16_t y){
    char string[58];

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_BALL;
    string[5] = pick_notDrop;
    // Little endian representation
    string[6] = (uint8_t)(x);
    string[7] = (uint8_t)(x>>8);
    string[8] = (uint8_t)(y);
    string[9] = (uint8_t)(y>>8);

    /* Return number of bytes written */
    return write(s, string, 10);
}

/*  **************************** RECEIVING ****************************************/

/* Receive an ACK message from the server */
int bt_recv_ack(char * msg, ssize_t nbyte){
    if (msg[4] == MSG_ACK){
        printf("Received Ack message id=%u with status code=%u\n", *((uint16_t *) msg[5]), (uint16_t)msg[7]);
        return 0;
    } else {
        return -1;
    }
};

/* Receive a NEXT message from the server */
int bt_recv_next(char * msg, ssize_t nbyte){
    if (msg[4] == MSG_NEXT){
        printf("Recieved Next message\n");
        return 0;
    } else {
        return -1;
    }
};

/* Receive a START message from the server */
int bt_recv_start(char * msg, ssize_t nbyte, uint8_t *role, uint8_t *side, uint8_t *ally){
    if ( msg[4] == MSG_START) {
        *role = (uint8_t) string[5];
        *side = (uint8_t) string[6];
        *ally = (uint8_t) string[7];
        printf ("Received Start message with role=%u, side=%u, ally=%u!\n", *role, *side, *ally);
        return 0;
    } else {
        return -1;
    }
}

/* Receive a STOP message from the server */
int bt_recv_stop(char * msg, ssize_t nbyte){

    if (msg[4] == MSG_STOP) {
        printf ("Received Stop message!\n");
        return 0;
    } else {
        return -1;
    }
}

/* Receive a KICK message from the server */
int bt_recv_kick(char * msg, ssize_t nbyte){

    if (msg[4] == MSG_KICK) {
        printf ("Received Kick message for ID=%u!\n", msg[5]);
        return 0;
    } else {
        return = -1;
    }
}

/* Receive a BALL message from the server */
int bt_recv_ball(char * msg, ssize_t nbyte){
    char x[2], y[2];

    if (msg[4] == MSG_BALL) {
        x[0] = msg[7];
	x[1] = msg[6];
        y[0] = msg[9];
	y[1] = msg[8];
        printf ("Received Ball message with Ball=%u, X=%d, Y=%d!\n", (uint8_t)msg[5], (int16_t)x, (int16_t)y);
        return 0;
    } else {
        return = -1;
    }
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
