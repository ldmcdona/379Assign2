#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <time.h>

#define	MY_PORT	2224

/* ---------------------------------------------------------------------
   This	is  a sample server which opens a stream socket and then awaits
   requests coming from client processes.
   --------------------------------------------------------------------- */

int main(int argc, char * argv[])
{
	int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;
	int data[5];
        int board[100][2];

        int test = sizeof(board);
        int test2 = sizeof(data);

        int abort = 0;

        /*for(int i=0; i<100; i++){
          if((i > 0 && i < 9) || (i > 90 && i < 99)){
            board[i][0] = 1; //horizontal wall
          }else if((i % 10 == 0) || (i % 10 == 9)){
            board[i][0] = 2; //vertical wall
          }else{
            board[i][0] = 0; //empty space
          }
        }
        board[0][0] = 3;
        board[9][0] = 3;   //fence posts.
        board[90][0] = 3;
        board[99][0] = 3;*/

	int i = 0;

	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = inet_addr("127.0.0.1");
	master.sin_port = htons (MY_PORT);

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen (sock, 5);
	fromlength = sizeof (from);
	snew = accept (sock, (struct sockaddr*) & from, & fromlength);
	if (snew < 0) {
		perror ("Server: accept failed");
		exit (1);
	}
	outnum = htonl (number);
	
	// Zero out all of the bytes in character array c

        bzero(data,test2);
        bzero(board,test);

        while(abort == 0){
		
          recv(snew,data,test2,0);
          //recv(snew,board,100,0);

          for(int j=0; j<100; j++){
            if((j > 0 && j < 9) || (j > 90 && j < 99)){
              board[j][0] = 1; //horizontal wall
            }else if((j % 10 == 0) || (j % 10 == 9)){
              board[j][0] = 2; //vertical wall
            }else{
              board[j][0] = 0; //empty space
            }
          }
          board[0][0] = 3;
          board[9][0] = 3;   //fence posts.
          board[90][0] = 3;
          board[99][0] = 3;

          board[24][0] = 4; //enemy
          board[24][1] = 2;
          board[86][0] = 4;
          board[86][1] = 0;

          board[25][0] = 5; //user
          board[25][1] = 2; //facing south

          // printf("SERVER\n");
          /* for(int x=0; x<100; x++){
            printf("Tile %i is type %i\n", x, board[x][0]);
            }*/

          //printf("data[3] is %d\n", data[3]);
          
          switch(data[3]){
          case 1:
            printf("Join Game\n");
            break;
          case 2:
            printf("Move North\n");
            break;
          case 3:
            printf("Move East\n");
            break;
          case 4:
            printf("Move South\n");
            break;
          case 5:
            printf("Move West\n");
            break;
          case 6:
            printf("Fire\n");
            break;
          case 7:
            printf("Quit Game\n");
            abort = 1;
            break;
          }
		
          int test = sizeof(board);
		
          send(snew,board,test,0);

          bzero(data,test2);
          bzero(board,test);
	}
//
	close (snew);
	sleep(1);
}
