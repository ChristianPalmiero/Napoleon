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
#include "position.h"
#include <errno.h>
#include <sys/fcntl.h>
#include <pthread.h>

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

uint16_t msgId = 0x0000;
int s=0, arena;
bool moving=false;
uint8_t role, side, ally;    

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
    if(status!=0)
	printf("Error: %d\n", errno);    

    /* Return 0 if connected */
    return status;
}

int bt_close(){
    bt_stop_transmit();
    return close (s);
}

/* Read and check */
int bt_check(){
    ssize_t nbyte;
    char msg[58]; 
    
    /* Read from server */
    nbyte = read (s, msg, 10);

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
        switch(msg[4]) {
            case MSG_ACK :
                bt_recv_ack(msg);
                break;
            case MSG_NEXT :
                bt_recv_next(msg);
                break;
            case MSG_START :
                bt_recv_start(msg);
                break;
            case MSG_STOP :
                bt_recv_stop(msg);
                break;
            case MSG_KICK :
                bt_recv_kick(msg);
                break;
            case MSG_BALL :
                bt_recv_ball(msg);
                break;
            default :
                printf("BT: Invalid message\n");
                break;
        }
    }
    return nbyte;
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
ssize_t bt_send_next(){
    char string[58];

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_NEXT;

    moving=false;
    /* Return number of bytes written */
    return write(s, string, 5);
}


/* Send a POSITION message to the server */
ssize_t bt_send_position(){
    char string[58];
    float x, y;
    int heading;
    int16_t x1, y1;
   
    get_position_and_heading(&x, &y, &heading); 
    x1 = (int16_t)x;
    y1 = (int16_t)y;
    printf("Sending X: %d, Y:%d\n", x1, y1);

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    // Little endian representation
    string[5] = (uint8_t)(x1);
    string[6] = (uint8_t)(x1>>8);
    string[7] = (uint8_t)(y1);
    string[8]= (uint8_t)(y1>>8);

    /* Return number of bytes written */
    return write(s, string, 9);
}

/* Send a BALL message to the ally */
ssize_t bt_send_ball(){
    char string[58];
    float x, y;
    int heading;
    int16_t x1, y1;
    uint8_t pick_notDrop;
   
    get_position_and_heading(&x, &y, &heading); 
    x1 = (int16_t)x;
    y1 = (int16_t)y;

    if(role==0)
	pick_notDrop=0;
    else
	pick_notDrop=1;

    // Remember to increment msgId
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = ally;
    string[4] = MSG_BALL;
    string[5] = pick_notDrop;
    // Little endian representation
    string[6] = (uint8_t)(x1);
    string[7] = (uint8_t)(x1>>8);
    string[8] = (uint8_t)(y1);
    string[9] = (uint8_t)(y1>>8);

    /* Return number of bytes written */
    return write(s, string, 10);
}

/*  **************************** RECEIVING ****************************************/

/* Receive an ACK message from the server */
int bt_recv_ack(char * msg){
    if (msg[4] == MSG_ACK){
        printf("Received Ack message id=%u with status code=%u\n", *((uint16_t *) msg[5]), (uint16_t)msg[7]);
        return 0;
    } else {
        return -1;
    }
};

/* Receive a NEXT message from the server */
int bt_recv_next(char * msg){
    if (msg[4] == MSG_NEXT){
        printf("Recieved Next message\n");
        bt_send_ack(*((uint16_t *) msg), ally, 0);
	if(role==1){
	    moving=true;
	    //Big arena
	    if(arena==1)
		if(side==0)
                    arena_big_finisher(1);
                else
                    arena_big_finisher(-1);
    	    //Small arena 
	    else
		arena_small_finisher();
	}
        return 0;
    } else {
        return -1;
    }
};

/* Receive a START message from the server */
int bt_recv_start(char * msg){
    if ( msg[4] == MSG_START) {
        role = (uint8_t) msg[5];
        side = (uint8_t) msg[6];
        ally = (uint8_t) msg[7];
        printf ("Received Start message with role=%u, side=%u, ally=%u!\n", role, side, ally);
        if(role==0){
	    moving=true;
	//Big arena
	if(arena==1)
	    if(side==0)
                arena_big_beginner(1);
            else
                arena_big_beginner(-1);
	//Small arena
	else
	    arena_small_finisher();
	}
        return 0;
    } else {
        return -1;
    }
}

/* Receive a STOP message from the server */
int bt_recv_stop(char * msg){

    if (msg[4] == MSG_STOP) {
        printf ("Received Stop message!\n");
        bt_close();
        position_stop();
        engine_reset();
        ev3_uninit();
        return 0;
    } else {
        return -1;
    }
}

/* Receive a KICK message from the server */
int bt_recv_kick(char * msg){

    if (msg[4] == MSG_KICK) {
        printf ("Received Kick message for ID=%u!\n", msg[5]);
        return 0;
    } else {
        return -1;
    }
}

/* Receive a BALL message from the server */
int bt_recv_ball(char * msg){
    char x[2], y[2];

    if (msg[4] == MSG_BALL) {
        x[0] = msg[7];
	x[1] = msg[6];
        y[0] = msg[9];
	y[1] = msg[8];
        printf ("Received Ball message with Ball=%u, X=%d, Y=%d!\n", (uint8_t)msg[5], (int16_t)x, (int16_t)y);
        bt_send_ack(*((uint16_t *) msg), ally, 0);
        return 0;
    } else {
        return -1;
    }
}


/*  **************************** THREAD ****************************************/

bool bt_term = false;

void *bt_send(){
    while(!bt_term){
	if(moving)
             bt_send_position();
        sleep(2);
    }
    return NULL;
}

pthread_t t;

void bt_start_transmit(){
    bt_term = false;
    pthread_create(&t, NULL, bt_send, NULL);
}

void bt_stop_transmit(){
    bt_term = true;
    pthread_join(t, NULL);
}
