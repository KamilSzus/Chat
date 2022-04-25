OBJS	= Client.o Room.o Server.o User.o RoomFactory.o Chat.o
SOURCE	= Client.cpp ../sources/Room.cpp ../sources/Server.cpp ../sources/User.cpp ../sources/RoomFactory.cpp ../sources/Chat.cpp
HEADER	= ../headers/Room.h ../headers/Server.h ../headers/User.h ../headers/RoomFactory.h ../headers/Chat.h ClientMain.h
OUT	= ChatClient.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -pthread -lIce

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

Client.o: Client.cpp
	$(CC) $(FLAGS) Client.cpp -std=c++11

Room.o: ../sources/Room.cpp
	$(CC) $(FLAGS) ../sources/Room.cpp -std=c++11

Server.o: ../sources/Server.cpp
	$(CC) $(FLAGS) ../sources/Server.cpp -std=c++11

User.o: ../sources/User.cpp
	$(CC) $(FLAGS) ../sources/User.cpp -std=c++11

RoomFactory.o: ../sources/RoomFactory.cpp
	$(CC) $(FLAGS) ../sources/RoomFactory.cpp -std=c++11

Chat.o: ../sources/Chat.cpp
	$(CC) $(FLAGS) ../sources/Chat.cpp -std=c++11


clean:
	rm -f $(OBJS) $(OUT)