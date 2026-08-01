### Key Updates Made

* **CLI Options:** Standardized command flags (`--target` and `--ports`) across all examples to match your project's main interface.


* **Formatting:** Cleaned up code blocks, links, and overall Markdown readability.


* **Repository Structure:** Aligned paths and instructions for seamless deployment.



---

```markdown
# Simple Port Scanner

## What This Is

A concurrent TCP port scanner written in C++ that probes target hosts to identify open, closed, and filtered ports[cite: 3]. It uses asynchronous I/O to scan multiple ports simultaneously and attempts to grab service banners for fingerprinting[cite: 3].

## Why This Matters

Port scanning is the first step in almost every network security assessment and penetration test[cite: 3]. Before you can exploit a system, you need to know what's listening[cite: 3]. This tool teaches you how attackers enumerate network services and how defenders can detect such reconnaissance[cite: 3].

**Real-world scenarios where this applies:**

- **Penetration testing initial reconnaissance** — Every pentest starts with port scans to map the attack surface[cite: 3]. Tools like Nmap are standard, but understanding how they work under the hood makes you a better tester[cite: 3].
- **Security audit preparation** — Before a compliance audit (PCI-DSS, SOC 2), you need to verify which ports are exposed[cite: 3]. Unexpected open ports often indicate shadow IT or misconfigurations[cite: 3].
- **Incident response and threat hunting** — When investigating a breach, you scan internal networks to find backdoors, C2 channels, or lateral movement artifacts[cite: 3].

---

## What You'll Learn

This project teaches you how network reconnaissance works at the TCP layer[cite: 3]. By building it yourself, you'll understand:

### Security Concepts
- **Port states and their meanings:** The difference between `OPEN`, `CLOSED`, and `FILTERED` ports tells you about both the service and the firewall[cite: 3].
- **TCP connection mechanics:** Port scanning exploits the TCP three-way handshake (SYN, SYN-ACK, RST)[cite: 3].
- **Banner grabbing for fingerprinting:** Services often announce themselves via HTTP server headers or SSH version strings[cite: 3].

### Technical Skills
- **Asynchronous I/O programming:** Probing hundreds of ports concurrently using Boost.Asio instead of sequential blocking[cite: 3].
- **Concurrent programming patterns:** Managing multiple async operations with strand executors and smart pointers to prevent race conditions[cite: 3].
- **Network socket programming:** Low-level TCP socket operations below HTTP/application protocols[cite: 3].

---

## Prerequisites

### Required Knowledge
- **C++ Programming:** Familiarity with C++20 features, smart pointers (`std::shared_ptr`), and lambdas[cite: 3].
- **Networking Fundamentals:** IP addressing, port numbers, TCP vs. UDP, and the TCP 3-way handshake[cite: 3].
- **Terminal Comfort:** Basic command-line experience with `cmake` and `make`[cite: 3].

### Tools Required
- **CMake 3.31+:** Build configuration system[cite: 3].
- **C++20 Compiler:** GCC 10+, Clang 12+, or MSVC 2019+[cite: 3].
- **Boost Libraries:** `Boost.Asio` and `Boost.Program_options`[cite: 3].

---

## Quick Start

```bash
# Clone the repository
git clone [https://github.com/jenishraiyani5-oss/simple-port-scanner.git](https://github.com/jenishraiyani5-oss/simple-port-scanner.git)
cd simple-port-scanner

# Create build directory & compile
mkdir build && cd build
cmake ..
make

# Run the scanner on localhost
./simplePortScanner --target 127.0.0.1 --ports 1-1024

# Scan specific target with custom concurrency and timeout
./simplePortScanner --target scanme.nmap.org --ports 80,443,8080 --concurrency 50 --timeout 500

```

> **Expected Output:** A terminal table showing port number, status (`OPEN`/`CLOSED`/`FILTERED`), recognized service name, and grabbed banner strings.
> 
> 

---

## Project Structure

```
simple-port-scanner/
├── src/
│   ├── PortScanner.hpp      # Class definition, member variables, method signatures
│   └── PortScanner.cpp      # Core scanning logic, async operations, banner grabbing
├── main.cpp                 # Entry point, CLI argument parsing with boost::program_options
└── CMakeLists.txt           # Build configuration, dependencies (Boost)

```

---

## Next Steps

1. **Understand the concepts:** Read [01-CONCEPTS.md](https://www.google.com/search?q=./01-CONCEPTS.md) to learn about TCP port states and reconnaissance mechanics.


2. **Study the architecture:** Read [02-ARCHITECTURE.md](https://www.google.com/search?q=./02-ARCHITECTURE.md) to see how async I/O and concurrent scanning are designed.


3. **Walk through the code:** Read [03-IMPLEMENTATION.md](https://www.google.com/search?q=./03-IMPLEMENTATION.md) for a detailed explanation of the C++ & Boost.Asio implementation.


4. **Extend the project:** Read [04-CHALLENGES.md](https://www.google.com/search?q=./04-CHALLENGES.md) for ideas on UDP scanning, OS fingerprinting, and stealth techniques.



---

## Troubleshooting & Common Issues

**"boost/asio.hpp: No such file or directory"**

* **Linux:** `sudo apt install libboost-all-dev`

* **macOS:** `brew install boost`


**"Connection refused" on all ports**

* This is normal if scanning a target without active services listening. Try scanning `scanme.nmap.org` or local running web servers.



**Scanner hangs or runs slowly**

* Network firewalls may rate-limit high concurrency. Reduce `--concurrency` (e.g., `--concurrency 20`) or increase `--timeout`.



```

```