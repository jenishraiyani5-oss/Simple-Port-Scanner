### Key Improvements Made

* **CLI Options & References:** Updated all flag names and CLI examples (`--target`, `--ports`, `--concurrency`, `--timeout`) to match your project's main interface.


* **Code Logic Alignment:** Updated code snippets, string banners, and file paths to match your `PortScanner.hpp`, `PortScanner.cpp`, and `main.cpp` code structures.


* **Formatting:** Cleaned up code comments, guard clauses, memory safety examples, and pitfall highlights for maximum scannability.



---

```markdown
# Implementation Guide

This document walks through the actual source code, explaining how asynchronous port scanning works under the hood and highlighting the patterns that enable safe concurrent network I/O[cite: 6].

---

## File Structure Walkthrough


```

simple-port-scanner/
├── src/
│   ├── PortScanner.hpp     # Class definition: member variables, async I/O primitives, method signatures
│   └── PortScanner.cpp     # Implementation: async scan logic, completion handlers, banner grabbing
├── main.cpp                # Entry point: CLI parsing, scanner initialization, event loop execution
└── CMakeLists.txt          # Build config: C++20 standard, Boost dependency (Program_Options)

```

---

## 1. CLI Construction with Boost.Program_Options

```

### Option Definitions

In `main.cpp`, command-line flags are configured with type checking and default values[cite: 6]:

```cpp
po::options_description desc("Allowed options");
desc.add_options()
    ("help,h", "Produce help message")
    ("target,i", po::value<std::string>()->default_value("127.0.0.1"), "Target domain name or IP address")
    ("ports,p", po::value<std::string>()->default_value("1-1024"), "Port range (e.g., '80', '1-1024', '22,80,443')")
    ("concurrency,t", po::value<int>()->default_value(100), "Max concurrent scan worker operations")
    ("timeout,e", po::value<uint8_t>()->default_value(2)->value_name("sec"), "Connection timeout in seconds");
```[cite: 6]


### Displaying Usage Instructions

When requested via `-h` or `--help`, standard descriptions and practical user examples are rendered[cite: 6]:

```cpp
if (vm.count("help")) {
    std::cout << desc << "\n";
    std::cout << "Examples:\n"
              << "  Scan common ports on localhost:\n"
              << "    ./simplePortScanner --target 127.0.0.1 --ports 1-1024\n\n"
              << "  Full range port scan with custom concurrency:\n"
              << "    ./simplePortScanner --target 192.168.1.1 --ports 1-65535 --concurrency 200\n";
    return 0;
}
```[cite: 6]

---
```

## 2. Core Scanner Implementation

### Asynchronous Worker Routine

The primary recursive worker routine in `src/PortScanner.cpp` manages socket attempts and races them against deadline timers[cite: 6]:

```cpp
void PortScanner::scan() {
    // Guard clause: stop if queue is empty or concurrency limit reached
    if (q.empty() || cnt >= MAX_THREADS) return;  
    
    uint16_t port = q.front();
    q.pop();
    ++cnt;  // Track active operations
    
    auto socket = std::make_shared<tcp::socket>(io);
    auto timer = std::make_shared<boost::asio::steady_timer>(io);
    auto complete = std::make_shared<bool>(false);  // Race resolution flag
    
    tcp::endpoint endpoint(this->endpoint.address(), port);
    timer->expires_after(std::chrono::seconds(expiry_time));
    
    // Timer Handler: Races connection attempt
    timer->async_wait(boost::asio::bind_executor(strand, 
        [this, complete, socket, port](boost::system::error_code ec) {
            if (!ec && !*complete)  {
                *complete = true;
                socket->close();
                printf("%i\t%s\t%s\t%s\n", port, "FILTERED", "---", "---");
                ++filtered_ports;
                --cnt;
                scan();  // Recurse to process remaining queue entries
            }
        }));
    
    // Socket Handler: Connection attempt
    socket->async_connect(endpoint, boost::asio::bind_executor(strand,
        [this, socket, timer, port, complete](boost::system::error_code ec) {
            if (*complete) return;  // Timer won the race
            *complete = true;
            timer->cancel();       // Cancel pending deadline timer
            
            std::string service = "---";
            auto banner = std::make_shared<std::string>("---");
            
            auto it = basicPorts.find(port);
            if (it != basicPorts.end()) service = it->second;
            
            if (!ec) {
                // Connection Successful -> State: OPEN
                auto buf = std::make_shared<std::array<char, 128>>();
                socket->async_read_some(boost::asio::buffer(*buf),
                    boost::asio::bind_executor(strand,
                    [this, port, buf, banner, service](boost::system::error_code ec, std::size_t bytes_read) {
                        if (!ec && bytes_read > 0) {
                            banner->assign(buf->data(), bytes_read);
                        }
                        printf("%i\t%sOPEN%s\t%s\t%s\n", port, GREEN, RESET, service.c_str(), banner->c_str());
                        ++open_ports;
                        --cnt;
                        scan();
                    }));
            } else {
                // Connection Refused -> State: CLOSED
                printf("%i\t%sCLOSED%s\t%s\t%s\n", port, RED, RESET, service.c_str(), banner->c_str());
                ++closed_ports;
                --cnt;
                scan();
            }
        }));
}
```[cite: 6]

---

```

## 3. Key Technical Mechanics

### Memory Safety & Lifetime Management
Asynchronous operations outlive individual stack frame executions[cite: 6]. Dynamic instances for sockets, timers, buffers, and flags are allocated as `std::shared_ptr` objects[cite: 6]:

```cpp
auto socket = std::make_shared<tcp::socket>(io);
auto timer = std::make_shared<boost::asio::steady_timer>(io);
```[cite: 6]

Capturing smart pointers by value inside completion lambdas extends object life until all async handlers complete[cite: 6].

### Race Condition Prevention via Strands
`boost::asio::strand` synchronizes handler execution sequentially across asynchronous event iterations, eliminating data races when incrementing metrics (`open_ports`, `cnt`)[cite: 5, 6]:

```cpp
boost::asio::bind_executor(strand, handler_closure);
```[cite: 5, 6]

---
```

## 4. Port Specification & Parsing Logic

Target strings specified via `--ports` (e.g., `"1-1024"` or `"80"`) are parsed during initialization[cite: 3, 6]:

```cpp
void PortScanner::parse_port(std::string& port) {
    auto pos = port.find('-');
    if (pos == std::string::npos) {
        startPort = 1;
        endPort = static_cast<uint16_t>(std::stoi(port));
        return;
    }
    
    int start = std::stoi(port.substr(0, pos));
    int end = std::stoi(port.substr(pos + 1));
    
    if (start <= 0 || end > 65535 || start > end) {
        startPort = 1;
        endPort = 65535; // Fallback to full scanning range
    } else {
        startPort = static_cast<uint16_t>(start);
        endPort = static_cast<uint16_t>(end);
    }
}
```[cite: 6]

---
```

## 5. Common Implementation Hazards

### Pitfall 1: Unprotected Concurrent Mutations
* **Problem:** Modifying shared containers or counters inside callbacks without strand execution bindings[cite: 6].
* **Impact:** Intermittent memory corruption or unpredictable race conditions[cite: 6].
* **Fix:** Wrap every handler with `boost::asio::bind_executor(strand, ...)`[cite: 5, 6].

### Pitfall 2: Capturing Local Variables by Reference
* **Problem:** Capturing automatic stack variables by reference (`[&port]`) inside non-blocking lambdas[cite: 6].
* **Impact:** Use-after-free crashes when callbacks execute after outer scope destruction[cite: 6].
* **Fix:** Capture value types by value (`[port]`) and heap objects via `std::shared_ptr`[cite: 6].

---

## Build Commands

```bash
# Debug Build (Includes Debug Symbols)
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

# Release Build (Performance Optimized)
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```[cite: 6]

```
