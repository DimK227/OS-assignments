mngstd: main.o list.o hash.o
	gcc main.o hash.o list.o -o mngstd
main.o : main.c hash.h
	gcc -c main.c
hash.o : hash.c hash.h
	gcc -c hash.c
list.o : list.c list.h
	gcc -c list.c
clean :
	rm mngstd main.o hash.o list.o
