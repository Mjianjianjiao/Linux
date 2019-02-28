all: httpserver upload
httpserver: HttpServer.cpp ThreadPool.hpp Utils.hpp 
	g++  -o $@ $^ -g -std=c++11 -lpthread
upload: Upload.cpp
	g++ -o $@ $^ -g -std=c++11
.PHONY:clean
clean:
	rm -rf httpserver all upload
