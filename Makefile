all: read_file

read_file: read_file.c packet_decode.c reconstruct.c
	gcc -Wall -o read_file read_file.c packet_decode.c reconstruct.c

clean:
	rm read_file
