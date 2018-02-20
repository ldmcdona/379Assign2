#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <time.h>

#define	 MY_PORT  2224

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

int main()
{
	int	s, number;
	int data[5]; //1 moving, 1 shooting, 1 time, 2 spare
        int board[100][2]; //1 for type, 1 for facing
        //Client takes facing, shooting, and exit commands,
        //But it also has to handle the display. 
	struct	sockaddr_in	server;

	struct	hostent		*host;

	//time_t * seconds;

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}


	s = socket (AF_INET, SOCK_STREAM, 0);

	if (s < 0) {
		perror ("Client: cannot open socket");
		exit (1);
	}

	bzero (&server, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons (MY_PORT);

	if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}
	
	bzero(data,5);
        bzero(board,100);
 
	send(s,data,5,0);

	bzero(data,5);

        int test = sizeof(board);

        recv(s,board,test,0);
	sleep(2);

        int primer = 0;
        printf("CLIENT\n");
        for(int i=0; i<100; i++){
          if(board[i][0] == 1){
            printf("-");
          }else if(board[i][0] == 2){
            if(primer == 0){
              printf("|");
              primer = 1;
            }else{
              printf("|\n");
              primer = 0;
            }
          }else if(board[i][0] == 3){
             if(primer == 0){
               printf("+");
               primer = 1;
             }else{
               printf("+\n");
               primer = 0;
             }
          }else if(board[i][0] == 4){
            switch(board[i][1]){
            case 0:
              printf("^");
              break;
            case 1:
              printf(">");
              break;
            case 2:
              printf("v");
              break;
            case 3:
              printf("<");
              break;
            }
          }else if(board[i][0] == 5){
            char ESC=27;
            printf("%c[1m",ESC); //turn on bold
            switch(board[i][1]){
            case 0:
              printf("^");
              break;
            case 1:
              printf(">");
              break;
            case 2:
              printf("v");
              break;
            case 3:
              printf("<");
              break;
            }
            printf("%c[0m",ESC); //turn off bold
          }else{
            printf(" ");
            }
          }
        printf("\n");
        
	close (s);
}
