# Distributed Key-Value Store in C++ (with Boost.Asio)

This project is a simple **distributed key-value store** implemented in modern C++ using [Boost.Asio](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html) for TCP networking. It supports:

-  Basic operations: `PUT`, `GET`, `DEL`
- In-memory storage with **file-based persistence**
-  Primary-replica architecture with **real-time replication**
-  Thread-safe design using mutex
-  Simple and minimal design to learn distributed systems fundamentals

---

## 🔧 Architecture

Client --> Primary Server --> Replica 1
--> Replica 


- **Primary Server**: Listens for client commands over TCP, updates local store, and forwards changes to replicas.
- **Replicas**: Accept commands from the primary, update their local store, and persist to file.
- **Persistence**: Each node writes key-value pairs to its own `.txt` file.

---

## 🚀 Features

| Feature                  | Status |
|--------------------------|--------|
| PUT / GET / DELETE       | DONE   |
| File Persistence         | DONE   |
| Replication to 2 nodes   | DONE   |
| Multi-threaded Handling  | DONE   |
| Boost.Asio TCP Network   | DONE   |

---


## 🧪 Example Commands (from client)

PUT name Farmaan
GET name
DEL name

yaml
Copy
Edit

Each command ends with a newline (`\n`) and is sent over TCP.

---

## 🛠️ Building & Running

### 🧷 Prerequisites

- C++17 compatible compiler (e.g. g++, clang++)
- Boost libraries (specifically `boost::asio`)
- CMake (optional)

### 🧵 Step-by-Step

Compile with g++:

```bash
g++ server.cpp src/kvstore.cpp -o primary -lboost_system -lpthread
g++ replica_server1.cpp src/kvstore.cpp -o replica1 -lboost_system -lpthread
g++ replica_server2.cpp src/kvstore.cpp -o replica2 -lboost_system -lpthread
g++ client.cpp -o client -lboost_system -lpthread

🚦 Running the system
Open 4 terminals:

bash
Copy
Edit
# Terminal 1 - Start Replica 1
./replica1

# Terminal 2 - Start Replica 2
./replica2

# Terminal 3 - Start Primary Server
./primary

# Terminal 4 - Start Client
./client

📜 License
This project is licensed under the MIT License.

🙋 Author
Mohammad Farmaan Ansari

Feel free to reach out or fork/clone this repo for your own learning!

🌟 Contributions
This is a learning-focused project. Contributions, feedback, and suggestions are always welcome!