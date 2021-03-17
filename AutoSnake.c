#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define REFRESH_DELAY 45000L

typedef struct Position_ {
   int x;
   int y;
} Position;

typedef struct Cheese_ {
   Position posittion;
} Cheese;

typedef struct Player_ {
   Position posittion;
} Player;

void gameBorder(int x, int y, int height, int width);
Player* drawPlayer(Position posittion);
Cheese* drawCheese();
void startGame(Player* p, Cheese* c);
void pleaseWait(void);
int catchCheese(Player* p, Cheese* c);

int main(int argc, char const *argv[])
{
   int ch;
   Player* player;
   Cheese* cheese;
   Position p = { 0, 0};
	initscr();
   noecho();
	srand(time(NULL));

   // draw border of the game
   gameBorder(0, 0, 20, 30);
   player   = drawPlayer(p);
   cheese   = drawCheese();

   while( 1 )
   {
      startGame(player, cheese);
      getch();
   }

	refresh();
	endwin();
	return 0;
}

void gameBorder(int x, int y, int height, int width)
{
   int i;
   // draw top & bottom
   for(i = x; i < width; ++i)
      mvprintw(y, x+i, "-");
   for(i = x; i < width; ++i)
      mvprintw(y+height, x+i, "-");
   for(i = y; i < height; ++i)
      mvprintw(y+i, x, "|");
   for(i = y; i < y+height; ++i)
      mvprintw(y+i, x+width, "|");
}

Player* drawPlayer(Position posittion)
{
   Player* player    = malloc(sizeof(player));

   player->posittion.x     = posittion.x;
   player->posittion.y     = posittion.y;

   // FIXME seperate draw and data
   mvprintw(player->posittion.y, player->posittion.x, "o");
   return player;
}

Cheese* drawCheese()
{
   Cheese* cheese    = malloc(sizeof(Cheese));

   cheese->posittion.x     = 10;
   cheese->posittion.y     = 10;

   mvprintw(cheese->posittion.y, cheese->posittion.x, "@");
   //move(y, x);
   return cheese;
}

void startGame(Player* p, Cheese* c)
{
   while( (p->posittion.y != c->posittion.y) ||
         (p->posittion.x != c->posittion.x) )
   {
      catchCheese(p, c);
      pleaseWait();
   }
}

void pleaseWait(void)
{
   refresh();
   usleep(REFRESH_DELAY);
   fflush(stdout); // ???
}

int catchCheese(Player* p, Cheese* c)
{
   Position position;
   if( p->posittion.x < c->posittion.x )
   {
      p->posittion.x++;
      position    = p->posittion;
   }

   else if( p->posittion.x > c->posittion.x )
   {
      p->posittion.x--;
      position    = p->posittion;
   }

   else if( p->posittion.y < c->posittion.y )
   {
      p->posittion.y++;
      position    = p->posittion;
   }

   else if( p->posittion.y > c->posittion.y )
   {
      p->posittion.y--;
      position    = p->posittion;
   }

   else
   {
      return 0;      // faliure
   }

   drawPlayer(position);
}
