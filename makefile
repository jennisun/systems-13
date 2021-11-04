all: main.o
	gcc -o program main.o

main.o: main.c
	gcc -c main.c

clean:
	rm -rf main.o
	rm -rf rand_num.txt

run:
	./program
