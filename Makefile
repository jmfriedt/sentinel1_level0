all: read_file

read_file: read_file.c packet_decode.c
	gcc -Wall -o read_file read_file.c packet_decode.c

clean:
	rm read_file
