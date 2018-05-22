SOURCES := main.cpp link.cpp
OBJECTS := $(SOURCES:.cpp=.o)
INC := -I/usr/local/Cellar/OpenSSL/1.0.2o_1/include -I/usr/local/Cellar/mysql-connector-c++/1.1.9_1/include
LIB_DIR := -L/usr/local/Cellar/OpenSSL/1.0.2o_1/lib -L/usr/local/Cellar/mysql-connector-c++/1.1.9_1/lib
LIBS := -lcrypto -lmysqlcppconn

Link: $(OBJECTS)
	g++ $(OBJECTS) $(LIB_DIR) $(LIBS) -std=c++11 -o $@

%.o:%.cpp
	g++ $(CXXFLAGS) -c $^ $(INC) --std=c++11 -o $@ 

debug: CXXFLAGS += -DDEBUG -g
debug: Link