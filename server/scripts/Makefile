CC := gcc
Libs:= -lsqlite3 -lpthread #-ltcp_net_socket -L./tcp_net_socket 
CFLAGS := -Wall
Target := test
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += c_handle cmd cmd_reg server socket sql
AllObjs := $(addsuffix /src/*.o,$(Modules))
