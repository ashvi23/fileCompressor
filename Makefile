all: decompressor.c
decompressor: decompressor.c
	gcc -g -Wall -fsanitize=address decompressor.c -o decompressor
clean:
	rm -rf decompressor
