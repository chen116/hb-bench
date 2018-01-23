CC = gcc
CFLAGS = -O3 -Wall
LDFLAGS = 
LIBS = -lpoet -lhb-acc-pow-shared -lhb-energy-msr -lpthread -lrt -lm

all:  dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair

dijkstra_parallel_mqueue: dijkstra_parallel_mqueue.c
	$(CC) dijkstra_parallel_mqueue.c -o $@ $(LDFLAGS) $(LIBS)

dijkstra_parallel_squeue: dijkstra_parallel_squeue.c
	$(CC) dijkstra_parallel_squeue.c -o $@ $(LDFLAGS) $(LIBS)

dijkstra_allpair: dijkstra_allpair.c
	$(CC) dijkstra_allpair.c -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -rf dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair output*
