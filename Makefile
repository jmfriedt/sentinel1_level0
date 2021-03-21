all: read_file

read_file: read_file.c packet_decode.c reconstruct.c bypass.c
	gcc -O2 -o read_file read_file.c packet_decode.c reconstruct.c bypass.c -lm

clean:
	rm read_file
