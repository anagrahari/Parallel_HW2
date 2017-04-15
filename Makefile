CC=icpc
CFLAGS=-O3

all:
	$(CC) $(CFLAGS) -o par_randomized_cc par_randomized_cc.cpp 
	$(CC) $(CFLAGS) -o par_deterministic_cc par_deterministic_cc.cpp 
clean:
	rm -rf par_randomized_cc
	rm -rf par_deterministic_cc
