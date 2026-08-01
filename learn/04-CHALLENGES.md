### Key Updates Made to `04-CHALLENGES.md`

* **CLI Standardizations:** Aligned option flags across all challenges (`--target`, `--ports`, `--concurrency`, `--timeout`, `--output`).


* **Code Logic Alignment:** Updated line numbers and file signatures to reference your codebase (`src/PortScanner.cpp`, `main.cpp`).


* **Formatting:** Standardized progress indicators, task checklists, and architectural ASCII diagrams for scannability.



---

```markdown
# Extension Challenges

You've built a basic concurrent TCP port scanner[cite: 7]. Now make it production-ready with features that professional tools like Nmap have spent decades perfecting[cite: 7].

These challenges are ordered by difficulty[cite: 7]. Start with the easier ones to build confidence, then tackle the harder ones when you want to dive deeper[cite: 7].

---

## Easy Challenges

### Challenge 1: CSV Output Format

* **What to build:** Add a command line flag `-o` / `--output` that writes results to CSV instead of printing to terminal[cite: 7].
* **Why it's useful:** Security teams need machine-readable output for feeding into Excel or downstream reporting scripts[cite: 7].
* **Hints:**
  - Add standard flag option in `main.cpp`[cite: 6, 7]: `("output,o", po::value<std::string>(), "CSV output file path")`
  - Modify printing methods in `PortScanner::scan()` to append formatted lines[cite: 6, 7].
  - Format structure: `port,state,service,banner`[cite: 7].

**Test Execution:**
```bash
./simplePortScanner --target scanme.nmap.org --ports 1-1024 --output results.csv
cat results.csv
```[cite: 7]

---

### Challenge 2: Real-time Progress Indicator

* **What to build:** Show percentage completion during scans so users know execution status[cite: 7].
* **Why it's useful:** Large-range port scans can take time[cite: 7]. Continuous UI feedback prevents assuming the application froze[cite: 7].
* **Hints:**
  - Track `scanned_count` against `total_ports`[cite: 7].
  - Use carriage return (`\r`) to update lines in place[cite: 7]: `printf("\rProgress: %d/%d (%.1f%%)", scanned, total, percent);`[cite: 7]
  - Flush standard output via `fflush(stdout)`[cite: 7].

---

### Challenge 3: Batch Host Scanning

* **What to build:** Accept multiple comma-separated target hosts: `--target 192.168.1.1,192.168.1.2`[cite: 7].
* **Why it's useful:** Penetration tests require sweeping subnets rather than single IPs[cite: 7].
* **Hints:**
  - Add host range parsing logic in `PortScanner::parse_hosts()`[cite: 7].
  - Manage a vector of `tcp::endpoint` structures rather than a single destination[cite: 7].

---

## Intermediate Challenges

### Challenge 4: JSON Output Integration

* **What to build:** Add `--format json` support for structured security automation pipelines[cite: 7].
* **Hints:**
  - Integrate headers from `nlohmann/json`[cite: 7].
  - Store results in a standard vector before serializing on completion[cite: 7].

```json
{
  "target": "192.168.1.1",
  "ports_scanned": 1024,
  "results": [
    {"port": 22, "state": "OPEN", "service": "SSH", "banner": "SSH-2.0-..."},
    {"port": 80, "state": "CLOSED", "service": "HTTP", "banner": "---"}
  ]
}
```[cite: 7]

---

### Challenge 5: Active Service Version Probing

* **What to build:** Send protocol-specific probes (HTTP GET, FTP SYST) when initial banners are not actively broadcast[cite: 7].
* **Hints:**
  - Expand reading hooks inside `PortScanner.cpp` post-connection[cite: 6, 7].
  - Write protocol request sequences using `boost::asio::async_write`[cite: 7].

---

## Advanced Challenges

### Challenge 6: Stealth SYN Scanning (Half-Open Scan)

* **What to build:** Implement raw TCP SYN packet delivery without completing full TCP handshakes[cite: 7].


```

┌───────────┐
│  Kernel   │  ← Bypassed for sending, used for receiving
└─────┬─────┘
│
┌─────▼─────┐
│ Raw Socket│  ← Constructs SYN packets with raw headers
└─────┬─────┘
│
┌─────▼─────┐
│  Scanner  │  ← Intercepts SYN-ACK / RST flags
└───────────┘

```[cite: 7]

* **Requirements:** `SOCK_RAW` socket permissions (root/sudo access required)[cite: 5, 7].
* **Key Mechanics:** Manual TCP checksum generation and packet header assembly[cite: 7].

---

### Challenge 7: OS Fingerprinting

* **What to build:** Guess target operating systems based on TCP/IP stack characteristics (TTL, window sizes, flag sequences)[cite: 7].

---

## Expert Challenges

### Challenge 8: Full Engine & IDS Evasion

* **What to build:** Timing templates (`T0` through `T5`), decoy address spoofing, packet fragmentation, and rate limiters[cite: 7].

---

## Challenge Tracker

- [ ] Challenge 1: CSV Output[cite: 7]
- [ ] Challenge 2: Progress Indicator[cite: 7]
- [ ] Challenge 3: Batch Host Scanning[cite: 7]
- [ ] Challenge 4: JSON Output[cite: 7]
- [ ] Challenge 5: Service Version Probing[cite: 7]
- [ ] Challenge 6: Stealth SYN Scanning[cite: 7]
- [ ] Challenge 7: OS Fingerprinting[cite: 7]
- [ ] Challenge 8: Full Engine & Evasion[cite: 7]

```

---

# Consolidated `README.md` File

Here is the complete **`README.md`** file for your project root. It integrates the core concepts, setup, CLI flags, technical architecture, and documentation guide in a clean layout.

```markdown
# Simple Port Scanner (C++20 & Boost.Asio)

A high-performance, asynchronous TCP port scanner written in modern C++20 using Boost.Asio and Boost.Program_Options. 

This scanner demonstrates non-blocking network I/O, concurrent work queue management, and race-free state detection (OPEN, CLOSED, FILTERED) using strand-bound handlers and deadline timers.

---

## Features

- **Asynchronous Execution:** Driven by Boost.Asio `io_context` event loops for maximum speed.
- **Configurable Concurrency:** Control active worker operations to balance throughput and network resources.
- **Accurate State Detection:** Uses paired deadline timers to distinguish firewall-dropped packets (`FILTERED`) from active connection resets (`CLOSED`).
- **Basic Service & Banner Grabbing:** Resolves common service ports and extracts active banner responses upon connection.
- **Modern CLI:** Clean, type-safe command-line options parsed via `boost::program_options`.

---

## Project Structure & Documentation

The repository is structured into modular documentation files for deep-dive learning:

| File | Description |
| :--- | :--- |
| **`01-THEORY.md`** | Explains TCP handshakes, non-blocking I/O, event loops, and state detection logic. |
| **`02-ARCHITECTURE.md`** | High-level system architecture, component diagrams, data flow, and design patterns. |
| **`03-IMPLEMENTATION.md`** | Detailed walkthrough of the C++ codebase, Boost usage, strands, and memory safety. |
| **`04-CHALLENGES.md`** | Guided extension challenges ranging from CSV/JSON exporting to SYN stealth scanning. |

---

## Prerequisites & Dependencies

Before building, ensure you have the following installed:

- **C++ Compiler:** C++20 compliant (GCC 10+, Clang 11+, or MSVC 2019+)
- **Build System:** CMake 3.15+
- **Libraries:** Boost Libraries 1.70+ (specifically `system` and `program_options`)

### Installing Dependencies

**Ubuntu / Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev

```

**macOS (via Homebrew):**

```bash
brew install cmake boost

```

---

## Build Instructions

```bash
# 1. Clone the repository
git clone [https://github.com/your-username/simple-port-scanner.git](https://github.com/your-username/simple-port-scanner.git)
cd simple-port-scanner

# 2. Create build directory
mkdir -p build && cd build

# 3. Configure and compile
cmake -DCMAKE_BUILD_TYPE=Release ..
make

```

This compiles the executable `simplePortScanner` inside the `build/` directory.

---

## Usage & Command-Line Arguments

```text
Allowed options:
  -h, --help                Produce help message
  -i, --target arg (=127.0.0.1)
                            Set target domain name or IP address
  -p, --ports arg (=1-1024) Set port range (e.g., '80', '1-1024', '22,80,443')
  -t, --concurrency arg (=100)
                            Max concurrent thread worker operations
  -e, --timeout arg (=2)    Timeout duration in seconds

```

### Examples

**Scan common ports on localhost:**

```bash
./simplePortScanner --target 127.0.0.1 --ports 1-1024

```

**Full port scan with custom timeout and worker concurrency:**

```bash
./simplePortScanner --target 192.168.1.1 --ports 1-65535 --concurrency 200 --timeout 1

```

**Scan a remote domain:**

```bash
./simplePortScanner --target scanme.nmap.org --ports 20-100

```

---

## Example Output

```text
Port    State   Service Banner
--------------------------------------------------
22      OPEN    SSH     SSH-2.0-OpenSSH_8.9p1 Ubuntu-3ubuntu0.1
80      OPEN    HTTP    ---
443     OPEN    HTTPS   ---
8080    CLOSED  ---     ---
9200    FILTERED ---    ---

Scan complete: 3 OPEN, 1 CLOSED, 1 FILTERED (5 total ports scanned).

```

---

## Disclaimer

This tool is created for educational and security research purposes only. Scan only systems you own or have explicit written permission to test. Unauthorized port scanning can violate network policies and legal standards.

```

```