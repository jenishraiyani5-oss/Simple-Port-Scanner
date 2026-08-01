#ifndef PORTSCANNER_HPP
#define PORTSCANNER_HPP

#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <cstdio>
#include <string>
#include <unordered_map>

// ANSI Escape Codes for Terminal Color Output
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

using boost::asio::ip::tcp;

class PortScanner {
private:
    static const std::unordered_map<uint16_t, std::string> basicPorts;
    static const uint16_t MAX_PORT = 65535;

    boost::asio::io_context io;
    boost::asio::ip::tcp::resolver resolver{io};
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::strand<boost::asio::io_context::executor_type> strand{io.get_executor()};

    std::queue<uint16_t> q;
    int cnt = 0;
    int MAX_THREADS = 100;
    int open_ports = 0;
    int closed_ports = 0;
    int filtered_ports = 0;

    std::string domainName;
    uint16_t startPort = 1;
    uint16_t endPort = MAX_PORT;
    uint8_t expiry_time = 2;

    void scan();
    void setup_queue();
    void parse_port(std::string& port);

public:
    PortScanner();
    PortScanner(std::string& ip_address, std::string& port, int max_threads, uint8_t expiry_time);
    ~PortScanner() = default;

    void set_options(std::string& domainName, std::string& port, int max_threads, uint8_t expiry_time);
    void set_max_port(uint16_t port);
    void set_max_threads(int value);
    void set_ip_address(std::string ip);
    void set_expiry_time(uint8_t value);

    void start();
    void run();
};

#endif // PORTSCANNER_HPP