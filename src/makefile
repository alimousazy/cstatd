CC = clang++ 
CFLAGS = -I/usr/local/include -std=c++11  -I/usr/local/include/lua52 -g
LDIR = -L/usr/local/lib
LIBS = -lboost_system  -lboost_thread -lrocksdb -lpthread -llua
DEPS = connection.hpp server.hpp scatter.hpp heartbeat.hpp data_access.hpp point.h
OBJ  = connection.o server.o data_access.o main.o heartbeat.o


$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDIR) $(LIBS)

clean:
	rm -f *.o *~ core *~ 
