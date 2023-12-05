all: main.c Ext2Utilities.c Fat16Utilities.c Logic.c
	gcc -std=gnu11 -Wall -Wextra -lpthread main.c -o shooter Ext2Utilities.c Fat16Utilities.c Logic.c

install:
	./shooter /info Ext2
