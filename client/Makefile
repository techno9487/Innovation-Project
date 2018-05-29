SOURCES := main.cpp crypto.cpp
OBJECTS := $(SOURCES:.cpp=.o)
INC := -I/usr/local/Cellar/OpenSSL/1.0.2o_1/include
LIB_DIR := -L/usr/local/Cellar/OpenSSL/1.0.2o_1/lib
LIBS := -lcrypto

TEST_SOURCE = test.cpp crypto.cpp
TEST_OBJECTS := $(TEST_SOURCE:.cpp=.o)

Client: $(OBJECTS)
	g++ $(OBJECTS) $(LIB_DIR) $(LIBS) -std=c++11 -o $@

%.o:%.cpp
	g++ $(CXXFLAGS) -c $^ $(INC) --std=c++11 -o $@ 

debug: CXXFLAGS += -DDEBUG -g
debug: Client

Test: $(TEST_OBJECTS)
	g++ $(TEST_OBJECTS) $(LIB_DIR) $(LIBS) -std=c++11 -o $@