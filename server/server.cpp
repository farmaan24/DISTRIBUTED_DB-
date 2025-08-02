#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "../src/kvstore.hpp"
#include<thread>

using boost::asio::ip::tcp;

// Global variables for replica handling
std::vector<std::shared_ptr<tcp::socket>> replica_sockets;
std::mutex replica_mutex;

// Creates a shortcut

// Send command to all connected replicas
void replicate_command(const std::string& command) {
    std::lock_guard<std::mutex> lock(replica_mutex);
    for (auto& sock : replica_sockets) {
        if (sock && sock->is_open()) {
            boost::system::error_code ec;
            boost::asio::write(*sock, boost::asio::buffer(command + "\n"), ec);
            if (ec) {
                std::cerr << "Error sending to replica: " << ec.message() << "\n";
            }
        }
    }
}

// Accept and register a replica connection
void connect_to_replica(const std::string& host, int port, boost::asio::io_context& io_context) {
    try {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        auto sock = std::make_shared<tcp::socket>(io_context);
        boost::asio::connect(*sock, endpoints);
        std::cout << "Connected to replica: " << host << ":" << port << "\n";

        std::lock_guard<std::mutex> lock(replica_mutex);
        replica_sockets.push_back(sock);
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to replica at " << host << ":" << port << " - " << e.what() << "\n";
    }
}

void handle_client(tcp::socket socket,KeyValueStore& store){
    try{ boost::system::error_code error;
            
            for (;;)
            {
            
                boost::asio::streambuf buf;
               
                boost::asio::read_until(socket, buf, '\n', error);
                
                if (error)
                    break;

                std::istream is(&buf);
                
                std::string line;
                std::getline(is, line);
                
                std::cout << "Received: " << line << "\n";

                std::string response;
                std::istringstream iss(line);
                std::string command;
                iss >> command;
                std::transform(command.begin(), command.end(), command.begin(), ::tolower);
                if (command == "put")
                {
                    std::string key, value;
                    iss >> key;
                    std::getline(iss, value); // everything after line is value
                    if (key.empty() || value.empty())
                    {
                        response = "ERROR : Invalid PUT format\n";
                    }
                    else
                    {
                        value.erase(0, value.find_first_not_of(" \t")); // trim leading space
                        store.put(key, value);
                        replicate_command("put " + key + " " + value);
                        response = "OK\n";
                    }
                }
                else if (command == "del")
                {
                    std::string key;
                    iss >> key;
                    if (key.empty())
                    {
                        response = "ERROR: Missing key for DEL\n";
                    }
                    else
                    {
                        bool success = store.del(key);
                         if (success) {
                            replicate_command("del " + key); 
                                }
                        response = success ? "OK\n" : "NOT_FOUND\n";
                    }
                }
                else if(command=="get"){
                    std::string key ;
                    iss>>key ;
                    if(key.empty()){
                        response = "ERROR: Missing key for GET\n";
                    }else{
                        auto val = store.get(key);
                        response = val.has_value() ? val.value() + "\n" : "NOT_FOUND\n";
                    }
                }
                else if (command == "exit")
                {
                    response = "Goodbye\n";
                    boost::asio::write(socket, boost::asio::buffer(response), error);
                    break; // disconnect this client
                }
                else
                {
                    response = "ERROR: Unknown command\n";
                }
                boost::asio::write(socket, boost::asio::buffer(response), error);
               
                }
            }
                catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        KeyValueStore store("C:/Users/Mohammad farmaan/OneDrive/Desktop/projects/db-cpp/db-cpp/db/store_db.txt");
        store.load_from_disk();
        
        // Connect to replicas 
        connect_to_replica("127.0.0.1", 12346, io_context); // Replica 1
        connect_to_replica("127.0.0.1", 12347, io_context); // Replica 2

        std::cout << "Server running on port 12345...\n";

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            
            std::cout << "New Client connected.\n";
            std::thread(handle_client, std::move(socket), std::ref(store)).detach();
        
            }
       
            std::cout << "Client disconnected.\n";
        }
    
    catch (std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << "\n";
       
    }
    return 0;
}