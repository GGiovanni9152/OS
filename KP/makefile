FLAGS = -std=c++20 -lzmq -pthread -o

all: client server
client: client.cpp Client.cpp User.cpp
	g++ -fsanitize=address client.cpp Client.cpp User.cpp ${FLAGS} client -w

server: server.cpp Server.cpp User.cpp
	g++ -fsanitize=address server.cpp Server.cpp User.cpp ${FLAGS} server -w	
