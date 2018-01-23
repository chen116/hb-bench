
vic: clean
	gcc -O2 -Wall -c -o vic.o stream.c
	gcc -o vic vic.o -lhb-shared -lhrm-shared -lpthread
clean:
	-rm -f vic*