compile: t3.c udp.c udp.h
	gcc -o t3 t3.c udp.c
clean:
	rm t3
