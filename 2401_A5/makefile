


all: sim stop client
			gcc -o simulator simulator.o -lX11 -lm -lpthread
			gcc -o stop stop.o
			gcc -o robotClient robotClient.o -lX11 -lm -lpthread -g

stop: stop.c simulator.h
			gcc -c stop.c -g

sim: simulator.c simulator.h
			gcc -c simulator.c -lX11 -lm -lpthread -g

display: display.c
					gcc -c display.c -lX11 -lm -lpthread

client: robotClient.c simulator.h
				gcc -c robotClient.c

clean:
		rm -f *.o simulator stop
