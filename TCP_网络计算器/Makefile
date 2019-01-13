.PHONY:all
all: server client

server: server.hpp server.cc  socket.hpp thread_pool.hpp
	g++ -o $@ $^ -std=c++11 -g -lpthread
client: client.hpp client.cc socket.hpp
	g++ -o $@ $^ -std=c++11 -g -lpthread

.PHONY:clean
clean:
	rm -f all server client
