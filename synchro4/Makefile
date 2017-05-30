all: resource resource2 barber

barber:
	gcc -std=gnu99 -pthread barber.c -o barber

resource:
	gcc -std=c99 -pthread resource.c -o resource

resource2:
	gcc -std=c99 -pthread resource2.c -o resource2

clean:
	rm -f resource resource2 barber
