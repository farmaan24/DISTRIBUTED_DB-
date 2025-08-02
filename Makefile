
CXX = g++
# Add -pthread and static linking flags for thread support and portability
CXXFLAGS = -std=c++17 -Wall -static -static-libgcc -static-libstdc++ -pthread
# -std=c++17: Use the C++17 standard.
# -Wall: Enable all warnings.
# -Iinclude: Tells the compiler to look in the include/ folder for header files.

# Vcpkg integration on Windows
VCPKG_ROOT = C:/libraries/vcpkg/installed/x64-mingw-static
INCLUDES = -I$(VCPKG_ROOT)/include -Isrc
LIBS = $(VCPKG_ROOT)/lib/libboost_system-gcc15-mt-x64-1_88.a -lws2_32 -pthread
SRC = src/kvstore.cpp
# kvstore.cpp will contain the reusable KVStore class.
# This defines where the compiler should look for header files (*.h or *.hpp).
# -I$(VCPKG_ROOT)/include: Adds the Boost headers installed by vcpkg.
# Equivalent to: -IC:/libraries/vcpkg/installed/x64-mingw-static/include
# -Isrc: Adds your own src/ directory where your project's headers might be (like kvstore.hpp).
# This defines which libraries to link with during the linking stage of compilation.
# Let's break each part:
# $(VCPKG_ROOT)/lib/libboost_system-gcc15-mt-x64-1_88.a
# This is the static Boost.System library.
# It's used by Boost.Asio for low-level networking error handling, etc.
# -lws2_32
# This links the Windows Winsock2 networking library.
# Required for anything socket-related on Windows.
# -pthread
# This enables POSIX threading, useful for multi-threaded code.
# On MinGW, this helps enable C++11-style threads too (even though it’s Windows).

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
# To build the server executable, you need these dependencies:
# $(SERVER_SRC) → expands to server/server.cpp
# $(SRC) → expands to src/kvstore.cpp
# So it’s equivalent to writing:
# server: server/server.cpp src/kvstore.cpp
# $^ = all prerequisites = both files
# -o server: Output executable is ./server


$(CLIENT_BIN): $(CLIENT_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)
# Builds only from client/client.cpp
# This version of client may not yet need the kvstore, but we can include it later.


clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) $(REPLICA_BIN_1) $(REPLICA_BIN_2)


# This is a custom Makefile target named clean. It doesn't build any code — it performs a cleanup action. Developers often use it to remove temporary or compiled files.
# rm -f server client
# This is a shell command that:
# Deletes the server and client binaries (if they exist)
# -f = force (don’t complain if the files don’t exist)
# So, when you type:
# make clean
# It will remove:
# ./server
# ./client