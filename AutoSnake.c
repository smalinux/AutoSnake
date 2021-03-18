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
Player* drawPlayer(Position position, Position oldPosition);
Cheese* drawCheese(Position position);
void startGame(Player* p, Cheese* c);
void pleaseWait(void);
int catchCheese(Player* p, Cheese* c);

int main(int argc, char const *argv[])
{
   int ch;
   Player* player;
   Cheese* cheese;
   Position p = { 1, 1};
	initscr();
   noecho();
   curs_set(0);
	srand(time(NULL));

   // draw border of the game
   gameBorder(0, 0, 20, 30);
   player   = drawPlayer(p, p);

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

Player* drawPlayer(Position posittion, Position oldPosition)
{
   Player* player    = malloc(sizeof(player));

   player->posittion.x     = posittion.x;
   player->posittion.y     = posittion.y;

   // FIXME seperate draw and data
   mvprintw(player->posittion.y, player->posittion.x, "o");
   mvprintw(oldPosition.y, oldPosition.x, " ");
   return player;
}

Cheese* drawCheese(Position position)
{
   Cheese* cheese    = malloc(sizeof(Cheese));

   cheese->posittion.x     = position.x;
   cheese->posittion.y     = position.y;

   mvprintw(cheese->posittion.y, cheese->posittion.x, "@");
   return cheese;
}

void startGame(Player* p, Cheese* c)
{

   int x;
   int y;
   int ch;
   while(1)
   {
      c->posittion.x    = rand() % (30-2) + 1;
      c->posittion.y    = rand() % (20-2) + 2;
      if(mvinch(c->posittion.y, c->posittion.x) == 'o')
         continue;
      drawCheese(c->posittion);

      while( (p->posittion.y != c->posittion.y) ||
            (p->posittion.x != c->posittion.x) )
      {
         catchCheese(p, c);
         pleaseWait();
      }
   }

}

void pleaseWait(void)
{
   refresh();
   usleep(REFRESH_DELAY);
   //fflush(stdout); // ???
}

int catchCheese(Player* p, Cheese* c)
{
   Position position;
   static Position oldPos;
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

   drawPlayer(position, oldPos);
   oldPos = position;
}
