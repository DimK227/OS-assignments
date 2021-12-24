all: chef saladmaker
chef:
	gcc -o chef chef.c sem.c -lpthread
saladmaker:
	gcc -o saladmaker saladmaker.c sem.c -lpthread
clean:
	rm chef saladmaker logfile* file.txt
