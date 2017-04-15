CC=icpc
CFLAGS=-O3

all:
	$(CC) $(CFLAGS) -o par_randomized_cc par_randomized_cc.cpp 
clean:
	rm -rf par_randomized_cc
