all: read_file

read_file: read_file.c packet_decode.c reconstruct.c
	gcc -O2 -o read_file read_file.c packet_decode.c reconstruct.c -lm

clean:
	rm read_file
