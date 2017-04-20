all:
	gcc -std=c99 -pthread producer_consumer.c

clean:
	rm a.out
