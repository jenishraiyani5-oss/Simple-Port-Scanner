#include "src/PortScanner.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("target,i", po::value<std::string>()->default_value("127.0.0.1"), "set domain name or IP address")
            ("ports,p", po::value<std::string>()->default_value("1-1024"), "set a port range (e.g., 1-1024 or 80)")
            ("concurrency,t", po::value<int>()->default_value(100), "max concurrent threads")
            ("timeout,e", po::value<int>()->default_value(2)->value_name("sec"), "timeout in seconds")
            ("verbose,v", "verbose output");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            std::cout << "Examples:\n"
                      << "  Scan common ports on localhost:\n"
                      << "    ./simplePortScanner --target 127.0.0.1 --ports 1-1024\n\n"
                      << "  Full TCP port scan:\n"
                      << "    ./simplePortScanner --target 192.168.1.1 --ports 1-65535 --concurrency 200\n\n"
                      << "  Scan with custom timeout:\n"
                      << "    ./simplePortScanner --target example.com --ports 80-443 --timeout 5\n\n";
            return 0;
        }

        std::string ip = vm["target"].as<std::string>();
        std::string port = vm["ports"].as<std::string>();
        int threads = vm["concurrency"].as<int>();
        int expiry_time = vm["timeout"].as<int>();

        PortScanner scanner;
        scanner.set_options(ip, port, threads, static_cast<uint8_t>(expiry_time));
        scanner.start();
        scanner.run();

    } catch (const boost::program_options::error& e) {
        std::cerr << "Error parsing options: " << e.what() << "\n";
        std::cerr << "Use --help to see available arguments.\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}