files: client server

client: Client.cpp
	g++ -fsanitize=address Client.cpp -lzmq -o client -w

server: Server.cpp
	g++ -fsanitize=address Server.cpp -lzmq -o server -w
