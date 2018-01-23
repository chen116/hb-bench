
all: clean one two

one: 
	gcc -c -o vic1.o linpack1.c
	gcc -o vic1 vic1.o -lhb-shared -lhrm-shared
two:
	gcc -c -o vic2.o linpack2.c
	gcc -o vic2 vic2.o -lhb-shared -lhrm-shared
clean:
	-rm -f vic*