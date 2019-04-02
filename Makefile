CC = gcc
CCFLAGS = -Wall

all: fileCompressor

fileCompressor: fileCompressor.c fileCompressor.h 
	$(CC) $(CCFLAGS) -g -lm -o fileCompressor fileCompressor.c fileCompressor.h 
	
clean:
	rm fileCompressor



