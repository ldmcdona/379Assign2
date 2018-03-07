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

int q = 0;
char pressed;

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

void fire(int game[100][2], int person[2]);

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
  //printf("CLIENT\n");
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
    }else if(game[i][0] == 6){
      printf("O");
      game[i][0] = 0;
    }else{
      printf(" ");
    }
  }
}

int main() {
  int board[100][2];
  int user[2];
  int old;

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

  /*board[24][0] = 4; //enemy
  board[24][1] = 2;
  board[86][0] = 4;
  board[86][1] = 0;*/

  board[25][0] = 5; //user
  board[25][1] = 2; //facing south
  user[0] = 25;
  user[1] = 2;
  old = 25;

  printout(board);

  while(q == 0){
    keypress(0);
    switch(pressed){
    case 'i':
      user[1] = 0;
      if((user[0] / 10) > 1){
        user[0] = user[0] - 10;
      }
      break;
    case 'l':
      user[1] = 1;
      if((user[0] % 10) != 8){
        user[0] = user[0] + 1;
      }
      break;
    case 'k':
      user[1] = 2;
      if((user[0] / 10) < 8){
        user[0] = user[0] + 10;
      }
      break;
    case 'j':
      user[1] = 3;
      if((user[0] % 10) != 1){
        user[0] = user[0] - 1;
      }
      break;
    case ' ':
      // data[3] = 6;
      fire(board, user);
      break;
    case 'x':
      // data[3] = 7;
      q = 1;
      break;
    default:
      if(pressed == '\033'){
        keypress(0);
        keypress(0);
      }
      //printf("Invalid input, pressed is %c\n", pressed);
      break;
    }
    board[old][0] = 0;
    board[user[0]][0] = 5;
    board[user[0]][1] = user[1];
    old = user[0];
    printout(board);
  }
  return 0;
}

void fire(int board[100][2], int user[2]){
  switch(user[1]){
  case 0:
    if((user[0] / 10) == 1){
      break;
    }else if((user[0] / 10) == 2){
      board[user[0]-10][0] = 6;
    }else{
      board[user[0]-10][0] = 6;
      board[user[0]-20][0] = 6;
    }
    break;
  case 1:
    if((user[0] % 10) == 8){
      break;
    }else if((user[0] % 10) == 7){
      board[user[0]+1][0] = 6;
    }else{
      board[user[0]+1][0] = 6;
      board[user[0]+2][0] = 6;
    }
    break;
  case 2:
    if((user[0] / 10) == 8){
      break;
    }else if((user[0] / 10) == 7){
      board[user[0]+10][0] = 6;
    }else{
      board[user[0]+10][0] = 6;
      board[user[0]+20][0] = 6;
    }
    break;
  case 3:
        if((user[0] % 10) == 1){
      break;
    }else if((user[0] % 10) == 2){
      board[user[0]-1][0] = 6;
    }else{
      board[user[0]-1][0] = 6;
      board[user[0]-2][0] = 6;
    }
    break;
  }
}
