#include <ncurses.h>
#include <stdlib.h>

int max_y ;
int max_x ;

typedef struct Tile_S {
  char symbol ;
  bool walkable ;
} Tile ;

typedef struct Entity_S {
  char symbol ;
  int pos_y ;
  int pos_x ;
} Entity ;

Tile **map ;
Entity player ;
int key ;
int running ;

void curses_init(void) {
  initscr() ;
  raw() ;
  keypad(stdscr, true) ;
  noecho() ;
  getmaxyx(stdscr, max_y, max_x) ;
  curs_set(0) ;
  return ;
}

void create_tiles(void) {
  map = calloc(max_y, sizeof(Tile *)) ;

  for (int y = 0 ; y < max_y ; y++)
    map[y] = calloc(max_x, sizeof(Tile)) ;
  
  for (int y = 0 ; y < max_y ; y++)
    for (int x = 0 ; x < max_x ; x++) {
      map[y][x].symbol = '+' ;
      map[y][x].walkable = true ;
    }
}

void draw_map(void) {
  for (int y = 0 ; y < max_y ; y++)
    for (int x = 0 ; x < max_x ; x++) {
      mvaddch(y, x, map[y][x].symbol) ;
    }
  refresh() ;
}

void create_player() {
  player.pos_y = max_y/2 ;
  player.pos_x = max_x/2 ;
  player.symbol = '@' ;
}

void draw_entities() {
  mvaddch(player.pos_y, player.pos_x, player.symbol) ;
}

void action() {
  key = getch() ;

  switch(key) {
  case 'q':
    running = false ;
    break ;
  case 'w':
    --player.pos_y ;
    break ;
  case 'a':
    --player.pos_x ;
    break ;
  case 's':
    ++player.pos_y ;
    break ;
  case 'd':
    ++player.pos_x ;
    break ;
  default:
    break ;
  }

  if (player.pos_y < 0) {
    player.pos_y = max_y-1 ;
  } else if (player.pos_y >= max_y) {
    player.pos_y = 0 ;
  }

  if (player.pos_x < 0) {
    player.pos_x = max_x-1 ;
  } else if (player.pos_x >= max_x) {
    player.pos_x = 0 ;
  }
}

int main(void) {
  curses_init() ;
  create_tiles() ;
  create_player() ;

  running = true ;
  while (running) {
    draw_map() ;
    draw_entities() ;
    action() ;
  } ;
  
  endwin() ;
  return 0 ;
}
