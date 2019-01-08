.PHONY:all
all: server client

server: server.hpp server.cc  socket.hpp
	g++ -o $@ $^ -std=c++11 -g
client: client.hpp client.cc socket.hpp
	g++ -o $@ $^ -std=c++11 -g

.PHONY:clean
clean:
	rm -f all server client
