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
#include <termios.h>

#define	 MY_PORT  2224
int q = 0;
char pressed;

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

int keypress(unsigned char echo)
{
    struct termios savedState, newState;
    int c;

    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;     /* error on tcgetattr */
    }

    newState = savedState;

    if ((echo = !echo)) /* yes i'm doing an assignment in an if clause */
    {
        echo = ECHO;    /* echo bit to disable echo */
    }

    /* disable canonical input and disable echo.  set minimal input to 1. */
    newState.c_lflag &= ~(echo | ICANON);
    newState.c_cc[VMIN] = 1;

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;     /* error on tcsetattr */
    }

    c = getchar();      /* block (withot spinning) until we get a keypress */
    pressed = c;

    /* restore the saved state */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;     /* error on tcsetattr */
    }

    return c;
}

void printout(int game[100][2]){
  int primer = 0;
  printf("CLIENT\n");
  for(int i=0; i<100; i++){
    if(game[i][0] == 1){
      printf("-");
    }else if(game[i][0] == 2){
      if(primer == 0){
        printf("|");
        primer = 1;
      }else{
        printf("|\n");
        primer = 0;
      }
    }else if(game[i][0] == 3){
      if(primer == 0){
        printf("+");
        primer = 1;
      }else{
        printf("+\n");
        primer = 0;
      }
    }else if(game[i][0] == 4){
      switch(game[i][1]){
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
    }else if(game[i][0] == 5){
      char ESC=27;
      printf("%c[1m",ESC); //turn on bold
      switch(game[i][1]){
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
}

int main()
{
	int	s, number;
	int data[5]; //data 3 is key input, will probably use another for score.
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

        int test2 = sizeof(data);
 
	data[3] = 1;
//
	send(s,data,test2,0);

	bzero(data,test2);

        int test1 = sizeof(board);

        recv(s,board,test1,0);
	sleep(2);

        //printout(board);
        
        while(q == 0){
          keypress(0);
          switch(pressed){
          case 'i':
            data[3] = 2;
            break;
          case 'l':
            data[3] = 3;
            break;
          case 'k':
            data[3] = 4;
            break;
          case 'j':
            data[3] = 5;
            break;
          case ' ':
            data[3] = 6;
            break;
          case 'x':
            data[3] = 7;
            q = 1;
            break;
          default:
            printf("Invalid input\n");
            break;
          }
          send(s,data,test2,0);
          bzero(data,test2);
          recv(s,board,test1,0);
          sleep(2);
          //printout(board);
          // printf("CLIENT\n");
          // printf("Loop complete, pressed is %c\n", pressed);
        }
//        
	close (s);
}
