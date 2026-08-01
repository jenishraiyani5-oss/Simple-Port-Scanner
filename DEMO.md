
```markdown
<!-- DEMO.md -->

<div align="center">

```ruby
██████╗  ██████╗ ██████╗ ████████╗    ███████╗ ██████╗ █████╗ ███╗   ██╗███╗   ██╗███████╗██████╗
██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝    ██╔════╝██╔════╝██╔══██╗████╗  ██║████╗  ██║██╔════╝██╔══██╗
██████╔╝██║   ██║██████╔╝   ██║       ███████╗██║     ███████║██╔██╗ ██║██╔██╗ ██║█████╗  ██████╔╝
██╔═══╝ ██║   ██║██╔══██╗   ██║       ╚════██║██║     ██╔══██║██║╚██╗██║██║╚██╗██║██╔══╝  ██╔══██╗
██║     ╚██████╔╝██║  ██║   ██║       ███████║╚██████╗██║  ██║██║ ╚████║██║ ╚████║███████╗██║  ██║
╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝       ╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝

```

**Demo & Preview**

```bash
mkdir build && cd build && cmake .. && make
./simplePortScanner --target <target> --ports <range>

```

[SSH Discovery](https://www.google.com/search?q=%23ssh-discovery) · [HTTP Discovery](https://www.google.com/search?q=%23http-discovery)

---

### SSH Discovery

Async TCP scan against target host with verbose service mapping showing `OPEN`, `CLOSED`, and `FILTERED` states across the SSH well-known port range.

---

### HTTP Discovery

Concurrent scan across the HTTP port window with per-port service identification and aggregate result counts.

---

### Updated `README.md`

Here is your updated, production-ready **`README.md`** file for the root of your repository:

```markdown
# Simple Port Scanner (C++20 & Boost.Asio)

A high-performance, asynchronous TCP port scanner written in modern C++20 using Boost.Asio and Boost.Program_Options[cite: 1, 5].

This scanner demonstrates non-blocking network I/O, concurrent work queue management, and race-free state detection (`OPEN`, `CLOSED`, `FILTERED`) using strand-bound handlers and deadline timers[cite: 5, 6].

---

## Features

- **Asynchronous Execution:** Driven by Boost.Asio `io_context` event loops for high throughput[cite: 5].
- **Configurable Concurrency:** Control active worker operations to balance scan speed and network resources[cite: 5].
- **Accurate State Detection:** Uses paired deadline timers to distinguish firewall-dropped packets (`FILTERED`) from active connection resets (`CLOSED`)[cite: 5, 6].
- **Service & Banner Grabbing:** Resolves common service ports and extracts active banner responses upon connection[cite: 5, 6].
- **Modern CLI:** Clean, type-safe command-line options parsed via `boost::program_options`[cite: 5, 6].

---

## Quick Demo Preview

```bash
# Build and run a basic scan
mkdir build && cd build && cmake .. && make
./simplePortScanner --target 127.0.0.1 --ports 1-1024

```

For extended examples and previews, see [DEMO.md](https://www.google.com/search?q=./DEMO.md).

---

## Project Structure & Documentation

The repository includes detailed markdown guides covering design, implementation, and future extensions:

| File | Description |
| --- | --- |
| **[DEMO.md](https://www.google.com/search?q=./DEMO.md)** | Visual previews and basic usage quick-start.

 |
| **`01-THEORY.md`** | Explains TCP handshakes, non-blocking I/O, event loops, and state detection logic. |
| **`02-ARCHITECTURE.md`** | High-level system architecture, component diagrams, data flow, and design patterns.

 |
| **`03-IMPLEMENTATION.md`** | Detailed walkthrough of the C++ codebase, Boost usage, strands, and memory safety.

 |
| **`04-CHALLENGES.md`** | Guided extension challenges ranging from CSV/JSON exporting to SYN stealth scanning.

 |

---

## Prerequisites & Dependencies

Before building, ensure you have the following installed:

* **C++ Compiler:** C++20 compliant (GCC 10+, Clang 11+, or MSVC 2019+)


* **Build System:** CMake 3.15+


* **Libraries:** Boost Libraries 1.70+ (specifically `system` and `program_options`)



### Installing Dependencies

**Ubuntu / Debian:**

```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev
```[cite: 6]

**macOS (via Homebrew):**
```bash
brew install cmake boost
```[cite: 6]

---

## Build Instructions

```bash
# 1. Clone the repository
git clone [https://github.com/jenishraiyani5-oss/simple-port-scanner.git](https://github.com/jenishraiyani5-oss/simple-port-scanner.git)
cd simple-port-scanner

# 2. Create build directory
mkdir -p build && cd build

# 3. Configure and compile
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```[cite: 6, 9]

This compiles the executable `simplePortScanner` inside the `build/` directory[cite: 6].

---

## Usage & Command-Line Options

```text
Allowed options:
  -h, --help                    Produce help message
  -i, --target arg (=127.0.0.1) Target domain name or IP address
  -p, --ports arg (=1-1024)     Port range (e.g., '80', '1-1024', '22,80,443')
  -t, --concurrency arg (=100)  Max concurrent scan worker operations
  -e, --timeout arg (=2)        Connection timeout in seconds
```[cite: 6]

### Usage Examples

**Scan common ports on localhost:**
```bash
./simplePortScanner --target 127.0.0.1 --ports 1-1024
```[cite: 6, 9]

**Full range port scan with custom timeout and worker concurrency:**
```bash
./simplePortScanner --target 192.168.1.1 --ports 1-65535 --concurrency 200 --timeout 1
```[cite: 5, 6]

**Scan a remote host:**
```bash
./simplePortScanner --target scanme.nmap.org --ports 20-100
```[cite: 6, 7]

---

## Example Terminal Output

```text
Port    State   Service Banner
--------------------------------------------------
22      OPEN    SSH     SSH-2.0-OpenSSH_8.9p1 Ubuntu-3ubuntu0.1
80      OPEN    HTTP    ---
443     OPEN    HTTPS   ---
8080    CLOSED  ---     ---
9200    FILTERED ---    ---

Scan complete: 3 OPEN, 1 CLOSED, 1 FILTERED (5 total ports scanned).
```[cite: 6]

---

## Disclaimer

This tool is created strictly for educational and security research purposes. Only scan systems you own or have explicit authorization to test[cite: 6]. Unauthorized port scanning can violate organizational network policies and applicable law[cite: 6].

```