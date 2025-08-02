
CXX = g++

CXXFLAGS = -std=c++17 -Wall -static -static-libgcc -static-libstdc++ -pthread


# Vcpkg integration on Windows
VCPKG_ROOT = C:/libraries/vcpkg/installed/x64-mingw-static
INCLUDES = -I$(VCPKG_ROOT)/include -Isrc
LIBS = $(VCPKG_ROOT)/lib/libboost_system-gcc15-mt-x64-1_88.a -lws2_32 -pthread
SRC = src/kvstore.cpp

# Output binaries
SERVER_BIN = server.exe
CLIENT_BIN = client.exe

REPLICA_BIN_1 = replica_server1.exe
REPLICA_SRC_1 = server/replica_server1.cpp

REPLICA_BIN_2 = replica_server2.exe
REPLICA_SRC_2 = server/replica_server2.cpp

SERVER_SRC = server/server.cpp
CLIENT_SRC = client/client.cpp
# Define the client and server source files

all: $(SERVER_BIN) $(CLIENT_BIN) $(REPLICA_BIN_1) $(REPLICA_BIN_2)
# This is the default target — so running make will build both server and client.
# Rules to build executable


$(SERVER_BIN): $(SERVER_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(REPLICA_BIN_1): $(REPLICA_SRC_1) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

$(REPLICA_BIN_2): $(REPLICA_SRC_2) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)



$(CLIENT_BIN): $(CLIENT_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) $(REPLICA_BIN_1) $(REPLICA_BIN_2)


