// TODO: Head & Cheese same structure!! rename & clean & use 1 .. 
// TODO: el hardcode
// TODO: HARDCODE
// TODO: use every single ncurse API : ))
// TODO: el usleep worning
// TODO: handle el 'q' option!
// TODO: Free el memory allocations && GameOver function


#include "stdlib.h"
#include "ncurses.h"
#include "time.h"
#include "unistd.h"

int g_score = 0;
// 2D array of all spaces on the board.
bool *spaces;

typedef struct Snake
{
	struct Head * head;
	int length;
} Snake;

typedef struct Position
{
	int y;
	int x;	
} Position;

typedef struct Head
{
	char symbol;
	Position * position;
	struct Head * next;
	struct Head * prev;
} Head;

 Head *front=NULL;
 Head *back=NULL;

typedef struct Cheese
{
	char symbol;
	Position * position;
} Cheese;



// Head functions
Head * headSetUp(Position * position);
int headDraw(Head * head);

// Target functions
Cheese * cheeseSetUp();
int cheeseDraw(Cheese * cheese);

// Bascic Move
int headStepMove(Position * headPos, Position * cheesePos);
int headErase(Head * head);

// Check if Head == Cheese Position
int Arrived(Position * headPos, Position * cheesePos);

Position* peek( );
Position* dequeue( );
void enqueue( Position * position );
void snake_write_text( int y, int x, char* str );
void snake_game_over( );
bool snake_in_bounds( Position * position );
int snake_cooridinate_to_index( Position * position );
Position snake_index_to_coordinate( int index );
bool snake_move_player( Position * head, Position * cheesePos );


// =========================================================================
int main(int argc, char const *argv[])
{
	int ch;
	Head * head;

	// Set up the 2D array of all spaces
    spaces = (bool*) malloc( sizeof( bool ) * 30 * 30 );

	Cheese * cheese;

	initscr();
	srand(time(NULL));


	Position * position;
	position = malloc(sizeof(Position));

	position->y = 5;
	position->x = 5;
	head 	= headSetUp(position);



	// headDraw(head);

	cheese = cheeseSetUp();
	cheeseDraw(cheese);
	
	noecho();
	enqueue( head->position );

	while(ch != 'q') {
		headStepMove(head->position, cheese->position);
		// headDraw(head);
		snake_move_player( head->position, cheese->position );
		if ( Arrived(head->position, cheese->position) )
		{
			cheeseDraw(cheese);
			// headErase(head);
			if( !snake_in_bounds( head->position ) )
			    1;
			else
			    snake_move_player( head->position, cheese->position );
			continue;
		} else {
			move(cheese->position->y, cheese->position->x); 		// move curser to the cheese!
			refresh();
			usleep(80000);
			// headErase(head);
			if( !snake_in_bounds( head->position ) )
			    1;
			else
			    snake_move_player( head->position, cheese->position );
			continue;
		}
		
		ch = getch();
	}
	
	refresh();
	endwin();
	return 0;
}
// =========================================================================

Head * headSetUp(Position * position) 
{
	Head * head;
	head 					= malloc(sizeof(Head));
	head->position 			= malloc(sizeof(Position));

	head->position->y 		= position->y;
	head->position->x 		= position->x;
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
	cheese->position->y 	= rand() % 30;		// HARDCODE
	cheese->position->x 	= rand() % 30;		// HARDCODE
	mvprintw(cheese->position->y, cheese->position->x, "@");
	move(cheese->position->y, cheese->position->x);
	return 1;
}

int headStepMove(Position * headPos, Position * cheesePos) {
	// Move Down .. Down .. !
	if (abs((headPos->y +1) - cheesePos->y) < abs( (headPos->y) - cheesePos->y) && 
		mvinch(headPos->y +1, headPos->x) != '*')
	{
		++headPos->y;
	}

	// Move Up
	else if (abs((headPos->y -1) - cheesePos->y) < abs( (headPos->y) - cheesePos->y) && 
		mvinch(headPos->y -1, headPos->x) != '*')
	{
		--headPos->y;
	}

	// Move Left
	else if (abs((headPos->x -1) - cheesePos->x) < abs( (headPos->x) - cheesePos->x) && 
		mvinch(headPos->y, headPos->x -1) != '*')
	{
		--headPos->x;
	}

	// Move Right
	else if (abs((headPos->x +1) - cheesePos->x) < abs( (headPos->x) - cheesePos->x) && 
		mvinch(headPos->y, headPos->x +1) != '*')
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

/** Start linked-list functions
========================================================================= */

// Returns the position at the front and dequeues
Position* dequeue( )
{
    Head *oldfront 	= front;
    front 			= front->next;

    return oldfront->position;
}

// Queues a position at the back
void enqueue( Position * position )
{
   Position *newpos   	= malloc( sizeof( position ) ); 
   Head *newnode 		= malloc( sizeof( Head ) );

   newpos->x = position->x;
   newpos->y = position->y;
   newnode->position = newpos;

   if( front == NULL && back == NULL )
       front = back = newnode;
   else
   {
       back->next = newnode;
       newnode->prev = back;
       back = newnode;
   }
}

// =========================================================================

// Is the current position in bounds?
bool snake_in_bounds( Position * position )
{
    return position->y < 30 - 1 && position->y > 0 && position->x < 30 - 1 && position->x > 0;
}


// Handles moving the snake for each iteration
bool snake_move_player( Position * head, Position * cheesePos )
{   
    enqueue( head );
        
    // Check if we're eating the fruit
    if( !Arrived(head, cheesePos) ) {
    	Position *tail = dequeue( );
        mvprintw( tail->y, tail->x, " " );
    }
    
    // Draw the new head 
    mvprintw( head->y, head->x, "S" );
}

/** End linked-list functions
------------------------------------------------------------------------- */







