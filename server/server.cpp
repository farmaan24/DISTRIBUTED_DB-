#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "../src/kvstore.hpp"

using boost::asio::ip::tcp;
// Creates a shortcut

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        KeyValueStore store;

        std::cout << "Server running on port 12345...\n";

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            // BLOCKS here waiting for a client to connect. When a client connects:
            // The connection is established
            // The socket object now represents the connection to that specific client
            // Program continues to next line
            std::cout << "Client connected.\n";

            boost::system::error_code error;
            // Creates an error code object to capture any I/O errors without throwing exceptions.

            for (;;)
            {
                // Infinite loop to handle multiple messages from the same client.
                boost::asio::streambuf buf;
                // Creates a buffer to store incoming data. streambuf is a stream-based buffer that can grow dynamically.
                boost::asio::read_until(socket, buf, '\n', error);
                // BLOCKS reading from the client until:
                // A newline character (\n) is received, OR
                // An error occurs (like client disconnecting)
                // Data is stored in buf
                // Any error is stored in error (doesn't throw exception)
                if (error)
                    break;

                std::istream is(&buf);
                // Creates an input stream from the buffer so you can read from it like std::cin.
                std::string line;
                std::getline(is, line);
                // Extracts the received message from the stream into a string, removing the newline character.
                std::cout << "Received: " << line << "\n";

                std::string response = "ACK: " + line + "\n";
                // Creates a response string by prepending "ACK: " to the received message and adding a newline.
                boost::asio::write(socket, boost::asio::buffer(response), error);
                // BLOCKS while sending the response back to the client:
                // boost::asio::buffer(response) wraps the string for sending
                // Writes all data to the socket
                // Stores any error in error
                if (error)
                    break;

                
            }

            std::cout << "Client disconnected.\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << "\n";
        // Catches any exceptions that occur and prints the error message.
        // std::cerr is the standard error stream in C++, 
        // similar to std::cout but specifically for error messages.
        // e.what() is a method that returns a description of what went wrong in an exception.
    }
    return 0;
}