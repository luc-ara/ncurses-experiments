#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define TICK 50 //duration of a turn
#define SNAKE_MAX 256

typedef struct Point_S {
  int y ;
  int x ;
} Point ;

int maxY ;
int maxX ;
int input = '\0' ;
int direction = 'd' ;
int running = 1 ;
int snakeLength = 3 ;
int score = 0 ;

Point food ;
Point snake[SNAKE_MAX] ;
Point corner ;

void createSnake(void) {
  snake[0].y = corner.y/2 ;
  snake[0].x = corner.x/2 ;

  snake[1].y = corner.y/2 ;
  snake[1].x = corner.x/2 - 1 ;
  
  snake[2].y = corner.y/2 ;
  snake[2].x = corner.x/2 - 2 ; 
}

void createFood(void) {
  food.y = (rand()%(corner.y-1))+1 ;
  food.x = (rand()%(corner.x-1))+1 ;
}

void drawWalls() {
  mvaddch(0, 0, '+') ;
  mvaddch(0, corner.x, '+') ;
  mvaddch(corner.y, 0, '+') ;
  mvaddch(corner.y, corner.x, '+') ;


  for (int y = 1 ; y < corner.y; ++y) {
    mvaddch(y, 0, '|') ;
    mvaddch(y, maxY-1, '|') ;
  }
  for (int x = 1 ; x < corner.y; ++x) {
    mvaddch(0, x, '-') ;
    mvaddch(maxY-1, x, '-') ;
  }
}

void getInput(void) {  
  input = getch() ;

  switch (input) {
    case 'q':
      running = 0 ;
      break ;
    case 'w':
      if (direction != 's') {
        direction = 'w' ;
      }
      break ;
    case 'a':
      if (direction != 'd') {
        direction = 'a' ;
      }
      break ;
    case 's':
      if (direction != 'w') {
        direction = 's' ;
      }
      break ;
    case 'd':
      if (direction != 'a') {
        direction = 'd' ;
      }
      break ;
    default:
      break ;
  }
 }

void moveSnake(void) {
  for (int s = snakeLength ; s > 0 ; --s) {
    snake[s].y = snake[s-1].y ;
    snake[s].x = snake[s-1].x ;
  }
  switch(direction) {
  case 'w':
    --snake[0].y ;
    if (snake[0].y == 0) {
      snake[0].y = corner.y-1 ;
    }
    break ;
  case 'a':
    --snake[0].x ;
    if (snake[0].x == 0) {
      snake[0].x = corner.x-1 ;
    }
    break ;
  case 's':
    ++snake[0].y ;
    if (snake[0].y == corner.y) {
      snake[0].y = 1 ;
    }
    break ;
  case 'd':
    ++snake[0].x ;
    if (snake[0].x == corner.x) {
      snake[0].x = 1 ;
    }
    break ;
  }
}

void checkCollision(void) {
  if (snake[0].y == food.y && snake[0].x == food.x) {
    score++ ;
    snakeLength++ ;
    createFood() ;
  }

  for (int s = 1 ; s < snakeLength ; s++) {
    if (snake[0].y == snake[s].y && snake[0].x == snake[s].x) {
      running = 0 ;
    }
  }
}

void draw(void) {
  clear() ;
  mvaddch(food.y, food.x, '*') ;
  mvprintw(0,0,"got here") ;

  for (int s = 0 ; s < snakeLength ; s++) {
    mvaddch(snake[s].y, snake[s].x, '@') ;
  }
  drawWalls() ;

  mvprintw(0, maxY+1, "%i", snakeLength) ;
  for (int s = 0 ; s < snakeLength ; s++) {
  mvprintw(s+1, maxY+1, "(%i, %i)", snake[s].y, snake[s].x) ;    
  }
  refresh() ;
}

int main(void) {
  srand(time(0)) ; //seed rng
  initscr() ; //start curses
  cbreak() ; //no line buffering
  noecho() ; //no echoing of typed characters
  curs_set(0) ; //no cursor
  timeout(TICK) ; //getch return every TICK seconds
  getmaxyx(stdscr, maxY, maxX) ; //get max Y and X dimensions
  corner.y = maxY-1 ;
  corner.x = maxY-1 ;
  
  createSnake() ; 
  createFood() ;

  while(running) {
    getInput() ;
    moveSnake() ;
    checkCollision() ;
    draw() ;
  }
  getch() ;

  //end ncurses
  nocbreak() ;
  endwin() ; 

  return 0 ;
}
