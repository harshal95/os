CC=gcc
CFLAGS=-Wall

537ps: option_processor.o read_options.o get_proc.o info_parser.o
	$(CC) $(CFLAGS) -o 537ps  option_processor.o read_options.o get_proc.o info_parser.o

option_processor.o: option_processor.c node_structs.h read_options.h get_proc.h info_parser.h
	$(CC) $(CFLAGS) -c option_processor.c

read_options.o: read_options.c node_structs.h read_options.h
	$(CC) $(CFLAGS) -c read_options.c

get_proc.o: get_proc.c node_structs.h get_proc.h
	$(CC) $(CFLAGS) -c get_proc.c

info_parser.o: info_parser.c node_structs.h
	$(CC) $(CFLAGS) -c info_parser.c
