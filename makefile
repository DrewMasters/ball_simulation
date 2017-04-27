CC = g++

default: server

server: ball_simulation.o server.o
	$(CC) -o server ball_simulation.o server.o

server.o: server.cpp server.h
	$(CC) -c server.cpp

ball_simulation.o: ball_simulation.cpp server.h
	$(CC) -c ball_simulation.cpp

clean:
	rm *.o server
