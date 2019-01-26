CURR_PATH = $(shell pwd)
SERVER = $(CURR_PATH)/Server
CLIENT =$(CURR_PATH)/Client
DATA_POOL=$(CURR_PATH)/data_pool
LIB=$(CURR_PATH)/lib
COMM=$(CURR_PATH)/Common
WINDOW=$(CURR_PATH)/Window


LD_FLAGS=-lpthread -lmyjson 
INCLUDE=-I$(DATA_POOL) -I$(LIB)/include -I$(COMM) -I$(WINDOW)
LIB_PATH=-L$(LIB)/lib 

cc=g++
ser_bin=chatSystem
cli_bin=chatClient


ser_obj=$(shell ls $(SERVER)/*.cc $(DATA_POOL)/*.cc $(COMM)/*.cc | sed -e 's/\/.*\///' -e 's/\.cc/\.o/')
cli_obj=$(shell ls $(CLIENT)/*.cc $(COMM)/*.cc $(WINDOW)/*.cc | sed -e 's/\/.*\///' -e 's/\.cc/\.o/')

.PHONY:all
all:$(ser_bin) $(cli_bin)

$(ser_bin):$(ser_obj)
	$(cc) -o $@ $^ $(LIB_PATH) $(INCLUDE) $(LD_FLAGS)

$(cli_bin):$(cli_obj)
	$(cc) -o $@ $^  $(LIB_PATH) $(INCLUDE) $(LD_FLAGS) -lncurses

%.o:$(SERVER)/%.cc 
	$(cc) -c $< $(INCLUDE)

%.o:$(CLIENT)/%.cc 
	$(cc) -c $< $(INCLUDE)

%.o:$(DATA_POOL)/%.cc 
	$(cc) -c $< $(INCLUDE)
%.o:$(COMM)/%.cc 
	$(cc) -c $<  $(INCLUDE)
%.o:$(WINDOW)/%.cc 
	$(cc) -c $<  $(INCLUDE)
.PHONY:clean
clean:
	rm -rf *.o $(ser_bin) $(cli_bin)
.PHONY:debug
debug:
	@echo $(ser_obj)
	@echo $(cli_obj)


