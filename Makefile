CC=icpc
CFLAGS=-O3
#OPTION=-I$(WORK)/cilktools-linux-004501/include/
OPTION=-I$(WORK)/cilkutil/include/
all:
	$(CC) $(CFLAGS) $(OPTION) -o par_randomized_cc par_randomized_cc.cpp 
	$(CC) $(CFLAGS) $(OPTION) -o par_deterministic_cc par_deterministic_cc.cpp 
clean:
	rm -rf par_randomized_cc
	rm -rf par_deterministic_cc
