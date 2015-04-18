CC = clang++ 
CFLAGS = -I/usr/local/include -std=c++11 `pkg-config --cflags protobuf`
LDIR = -L/usr/local/lib
LIBS = -lboost_system  -lboost_thread -lrocksdb `pkg-config --libs protobuf` -lpthread
DEPS = connection.hpp server.hpp scatter.hpp metric.hpp data_access.hpp point.h
OBJ  = connection.o server.o data_access.o main.o point.pb.o

protoc_generate: point.proto
	protoc --cpp_out=./ --java_out=./ --python_out=./ point.proto
	mv point.pb.cc point.pb.cpp

$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDIR) $(LIBS)

#build:  protoc_generate server
#
#.PHONY: clean
clean:
	rm -f *.o *~ core *~ 
