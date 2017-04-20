all:
	gcc -std=c99 -pthread mt19937ar.c producer_consumer.c

clean:
	rm a.out
