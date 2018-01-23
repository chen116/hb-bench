myapp=susan.c

all: clean
	gcc -c -o vic.o ${myapp}
	gcc -o vic vic.o -lhb-shared -lm -lhrm-shared -lpthread



# susan: susan.c Makefile
# 	gcc -static -O4 -o susan susan.c -lm 

clean:
	rm -f vic*
