INC := -I/usr/local/Cellar/OpenSSL/1.0.2l/include
LIB_DIR := -L/usr/local/Cellar/OpenSSL/1.0.2l/lib
LIBS := -lcrypto
SOURCES := sync.cpp

all:
	g++ $(SOURCES) $(INC) $(LIB_DIR) $(LIBS) -std=c++11 -o Sync
	