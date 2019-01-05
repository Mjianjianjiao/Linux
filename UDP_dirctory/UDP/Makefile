all: server client
server: server.cc server.hpp socket.hpp 
	g++ -o $@ $^ -std=c++11 -g  
client: client.cc client.hpp socket.hpp
	g++ -o $@ $^ -std=c++11 -g 

.PHONY:clean
clean:
	rm -f server client
