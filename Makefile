all:
	gcc -o main main.c
	$(MAKE) -C socket/
