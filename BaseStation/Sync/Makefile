INC := -I/usr/local/Cellar/OpenSSL/1.0.2o_1/include
LIB_DIR := -L/usr/local/Cellar/OpenSSL/1.0.20_1/lib
LIBS := -lcrypto
SOURCES := sync.cpp crypto.cpp

all:
	g++ $(SOURCES) $(INC) $(LIB_DIR) $(LIBS) -std=c++11 -o Sync
	