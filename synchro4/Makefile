all: resource barber

barber:
	gcc -std=gnu99 -pthread barber.c -o barber

resource:
	gcc -std=c99 -pthread resource.c -o resource

clean:
	rm -f resource barber
