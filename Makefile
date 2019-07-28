headers=  -lncurses -lpanel -lform -lmenu

all:
	gcc -o AutoSnake.o AutoSnake.c $(headers) -std=c99

clean:
	rm *.o