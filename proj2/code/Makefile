CC = gcc
CFLAGS = -Wall -Wextra
EXE = prodcomm
SCAN_BUILD_DIR = scan-build-out

all: mainProgram.o reader.o munch1.o munch2.o writer.o Queue.o 
	$(CC) -o $(EXE) mainProgram.o reader.o munch1.o munch2.o writer.o Queue.o -lpthread 

mainProgram.o: mainProgram.c Queue.h 
	$(CC) $(CFLAGS) -c mainProgram.c 

reader.o: reader.c Queue.h 
	$(CC) $(CFLAGS) -c reader.c

munch1.o: munch1.c Queue.h threadInput.h
	$(CC) $(CFLAGS) -c munch1.c

munch2.o: munch2.c Queue.h threadInput.h
	$(CC) $(CFLAGS) -c munch2.c

writer.o: writer.c Queue.h threadInput.h
	$(CC) $(CFLAGS) -c writer.c

Queue.o: Queue.c
	$(CC) $(CFLAGS) -c Queue.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html

 
