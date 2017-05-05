CC = g++
CFLAGS = -std=c++11

#default: server

all: server.o ball_simulation.o ball.o server

server: ball_simulation.o server.o
	$(CC) $(CFLAGS) -o server ball_simulation.o server.o

server.o: server.cpp server.h
	$(CC) $(CFLAGS) -c server.cpp

ball_simulation.o: ball_simulation.cpp server.h
	$(CC) $(CFLAGS) -c ball_simulation.cpp

ball.o: ball.cpp ball.h
	$(CC) $(CFLAGS) -c ball.cpp

clean:
	rm *.o server
