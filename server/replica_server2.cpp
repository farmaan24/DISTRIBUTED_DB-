#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "../src/kvstore.hpp"
#include<thread>

using boost::asio::ip::tcp;

void handle_replica(tcp::socket socket, KeyValueStore& store) {
    try {
        boost::system::error_code error;
        for (;;) {
            boost::asio::streambuf buf;
            boost::asio::read_until(socket, buf, '\n', error);

            if (error) break;

            std::istream is(&buf);
            std::string line;
            std::getline(is, line);

            std::cout << "Replica received: " << line << "\n";
            std::istringstream iss(line);
            std::string command;
            iss >> command;
            std::transform(command.begin(), command.end(), command.begin(), ::tolower);


            std::string key, value;

            if (command == "put") {
                iss >> key;
                std::getline(iss, value);
                value.erase(0, value.find_first_not_of(" \t"));
                if (!key.empty() && !value.empty()) {
                    store.put(key, value);
                    store.persist_to_disk();
                }
            } else if (command == "del") {
                iss >> key;
                if (!key.empty()) {
                    store.del(key);
                    store.persist_to_disk();
                }
            } else {
                std::cerr << "Invalid command received by replica 2\n";
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Replica 2 connection error: " << e.what() << "\n";
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12347));

        KeyValueStore store("C:/Users/Mohammad farmaan/OneDrive/Desktop/projects/db-cpp/db-cpp/db/replica2_db.txt");
        store.load_from_disk();

        std::cout << "Replica node running on port 12347...\n";

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Primary connected to replica 2 .\n";

            std::thread(handle_replica, std::move(socket), std::ref(store)).detach();
        }

    } catch (std::exception& e) {
        std::cerr << "Replica server 2 error: " << e.what() << "\n";
    }

    return 0;
}
