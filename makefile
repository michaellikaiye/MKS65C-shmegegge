all: s.o
	gcc s.o
s.o: s.c
	gcc -c s.c
clean:
	rm *.o
	rm a.out
run:
	./a.out
