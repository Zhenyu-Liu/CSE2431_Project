all:
	gcc -o main main.c
	gcc -o install install.c
	$(MAKE) -C socket/
