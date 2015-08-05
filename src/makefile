CC = clang++ 
CFLAGS = -I/usr/local/include -std=c++11 
LDIR = -L/usr/local/lib
LIBS = -lboost_system  -lboost_thread -lrocksdb -lpthread
DEPS = connection.hpp server.hpp scatter.hpp heartbeat.hpp data_access.hpp point.h
OBJ  = connection.o server.o data_access.o main.o 


$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDIR) $(LIBS)

clean:
	rm -f *.o *~ core *~ 
