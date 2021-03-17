headers=  -lncurses -lpanel -lform -lmenu

all:
	gcc -o AutoSnake.o AutoSnake.c $(headers)

clean:
	rm *.o
