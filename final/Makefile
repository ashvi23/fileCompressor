all: point
point: pointersorter.c
	gcc -g -Wall -Werror -fsanitize=address -std=c99 pointersorter.c -o pointersorter
clean:
	rm -rf point
