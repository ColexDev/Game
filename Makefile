main:
	gcc -I/usr/local/include src/main.c -L/usr/local/lib -Wl,-R/usr/local/lib -lncurses -lpthread -o game
