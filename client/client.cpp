#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        // Tries to connect to the server using the resolved endpoints
        // If successful, socket now represents the connection to the server
        // If server isn't running, this will throw an exception
        std::cout << "Connected to server.\n";

        while (true)
        {
            std::string msg;
            std::cout << "> ";
            std::getline(std::cin, msg);
            if (msg.empty())
                continue;

            msg += "\n";
            boost::asio::write(socket, boost::asio::buffer(msg));
            // Converts msg string to buffer view
            // Sends all bytes through the socket
            // Waits until complete message is sent
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, '\n');

            std::istream is(&response);
            std::string reply;
            std::getline(is, reply);

            std::cout << "Server: " << reply << "\n";

            if (msg == "EXIT\n")
                break;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Client error: " << e.what() << "\n";
    }
    return 0;
}