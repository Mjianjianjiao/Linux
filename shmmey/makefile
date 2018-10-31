all:reader writter
reader:reader.c
	gcc -o $@ $^
writter:writter.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f all reader writter 
