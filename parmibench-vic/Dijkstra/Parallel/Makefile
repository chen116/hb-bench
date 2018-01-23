

all:  dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair

dijkstra_parallel_mqueue: dijkstra_parallel_mqueue.c
	gcc -pthread dijkstra_parallel_mqueue.c -o dijkstra_parallel_mqueue -lhb-shared -lhrm-shared

dijkstra_parallel_squeue: dijkstra_parallel_squeue.c
	gcc -pthread dijkstra_parallel_squeue.c -o dijkstra_parallel_squeue

dijkstra_allpair: dijkstra_allpair.c
	gcc -pthread dijkstra_allpair.c -o dijkstra_allpair

vic: clean
	gcc -pthread dijkstra_parallel_mqueue.c -o vic -lhb-shared -lhrm-shared



clean:
	-rm -rf vic* dijkstra_parallel_mqueue dijkstra_parallel_squeue dijkstra_allpair output*
