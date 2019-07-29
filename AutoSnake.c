// TODO: Head & Cheese same structure!! rename & clean & use 1 .. 
// TODO: el hardcode
// TODO: HARDCODE
// TODO: use every single ncurse API : ))

#include "stdlib.h"
#include "ncurses.h"
#include "time.h"

typedef struct Position
{
	int y;
	int x;	
} Position;

typedef struct Head
{
	char symbol;
	Position * position;
} Head;

typedef struct Cheese
{
	char symbol;
	Position * position;
} Cheese;



// Head functions
Head * headSetUp();
int headDraw(Head * head);

// Target functions
Cheese * cheeseSetUp();
int cheeseDraw(Cheese * cheese);

// Bascic Move
int headStepMove(Position * headPos, Position * cheesePos);
int headErase(Head * head);

// Check if Head == Cheese Position
int Arrived(Position * headPos, Position * cheesePos);

// -----------------------------------------------------------------------
int main(int argc, char const *argv[])
{
	int ch;
	Head * head;
	// head 					= malloc(sizeof(Head));
	// head->position 			= malloc(sizeof(Position));
	
	Cheese * cheese;
	// cheese 					= malloc(sizeof(Cheese));
	// cheese->position 		= malloc(sizeof(Position));

	initscr();
	srand(time(NULL));


	head = headSetUp();
	// headDraw(head);

	cheese = cheeseSetUp();
	cheeseDraw(cheese);
	
	noecho();

	while(ch != 'q') {
		headStepMove(head->position, cheese->position);
		headDraw(head);
		if ( Arrived(head->position, cheese->position) )
		{
			cheeseDraw(cheese);
		} else {
			move(cheese->position->y, cheese->position->x); 		// move curser to the cheese!
		}
		
		ch = getch();
		headErase(head);
	}
	
	refresh();
	endwin();
	return 0;
}
// -----------------------------------------------------------------------

Head * headSetUp() 
{
	Head * head;
	head 					= malloc(sizeof(Head));
	head->position 			= malloc(sizeof(Position));

	head->position->y 		= 1;
	head->position->x 		= 1;
	return head;
}

int headDraw(Head * head)
{
	mvprintw(head->position->y, head->position->x, "*");
	return 1;
}

Cheese * cheeseSetUp() 
{
	Cheese * cheese;
	cheese 					= malloc(sizeof(Cheese));
	cheese->position 		= malloc(sizeof(Position));

	return cheese;
}

int cheeseDraw(Cheese * cheese)
{
	cheese->position->y 	= rand() % 20;		// HARDCODE
	cheese->position->x 	= rand() % 20;		// HARDCODE
	mvprintw(cheese->position->y, cheese->position->x, "@");
	move(cheese->position->y, cheese->position->x);
	return 1;
}

int headStepMove(Position * headPos, Position * cheesePos) {
	// Move Down .. Down .. !
	if (abs((headPos->y +1) - cheesePos->y) < abs( (headPos->y) - cheesePos->y) /*&& mvinch(headPos->y +1, headPos->x) == '*'*/)
	{
		++headPos->y;
	}

	// Move Up
	else if (abs((headPos->y -1) - cheesePos->y) < abs( (headPos->y) - cheesePos->y) /*&& mvinch(headPos->y -1, headPos->x) == '*'*/)
	{
		--headPos->y;
	}

	// Move Left
	else if (abs((headPos->x -1) - cheesePos->x) < abs( (headPos->x) - cheesePos->x) /*&& mvinch(headPos->y, headPos->x -1) == '*'*/)
	{
		--headPos->x;
	}

	// Move Right
	else if (abs((headPos->x +1) - cheesePos->x) < abs( (headPos->x) - cheesePos->x) /*&& mvinch(headPos->y, headPos->x +1) == '*'*/)
	{
		++headPos->x;
	}

	else { /* Do Nothing - REMOVE ME = else */
		mvprintw(0, 0, "CHECK ...");
	}

	return 1;
}
int headErase(Head * head)
{
	mvprintw(head->position->y, head->position->x, " ");
}

int Arrived(Position * headPos, Position * cheesePos)
{
	if (headPos->y == cheesePos->y && headPos->x == cheesePos->x)
	{
		return 1;
	}
	return 0;
}










