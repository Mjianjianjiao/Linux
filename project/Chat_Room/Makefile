CURR_PATH = $(shell pwd)
SERVER = $(CURR_PATH)/Server
CLIENT =$(CURR_PATH)/Client


LD_FLAGS=-lpthread
cc=g++
ser_bin=chatSystem
cli_bin=chatClient


ser_obj=$(shell ls $(SERVER)/*.cc | sed -e 's/\/.*\///' -e 's/\.cc/\.o/')
cli_obj=$(shell ls $(CLIENT)/*.cc | sed -e 's/\/.*\///' -e 's/\.cc/\.o/')

.PHONY:all
all:$(ser_bin) $(cli_bin)

$(ser_bin):$(ser_obj)
	$(cc) -o $@ $^ $(LD_FLAGS)

$(cli_bin):$(cli_obj)
	$(cc) -o $@ $^ $(LD_FLAGS)

%.o:$(SERVER)/%.cc 
	$(cc) -c $<

%.o:$(CLIENT)/%.cc 
	$(cc) -c $<

.PHONY:clean
clean:
	rm -rf *.o $(ser_bin) $(ser_bin)
.PHONY:debug
debug:
	@echo $(ser_obj)
	@echo $(cli_obj)


