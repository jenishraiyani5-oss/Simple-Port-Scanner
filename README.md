* **GitHub Handles & Links:** Updated original links to point directly to your GitHub profile (`jenishraiyani5-oss`).


* **Formatting & Structure:** Cleaned up section dividers, added clear **Prerequisites**, and organized the usage examples for better scannability.

* **Code Block Usability:** Fixed quick-start commands and standardized the just build runner notes.


---
```markdown
```ruby


██████╗  ██████╗ ██████╗ ████████╗    ███████╗ ██████╗ █████╗ ███╗   ██╗███╗   ██╗███████╗██████╗
██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝    ██╔════╝██╔════╝██╔══██╗████╗  ██║████╗  ██║██╔════╝██╔══██╗
██████╔╝██║   ██║██████╔╝   ██║       ███████╗██║     ███████║██╔██╗ ██║██╔██╗ ██║█████╗  ██████╔╝
██╔═══╝ ██║   ██║██╔══██╗   ██║       ╚════██║██║     ██╔══██║██║╚██╗██║██║╚██╗██║██╔══╝  ██╔══██╗
██║     ╚██████╔╝██║  ██║   ██║       ███████║╚██████╗██║  ██║██║ ╚████║██║ ╚████║███████╗██║  ██║
╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝       ╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝

```

> Asynchronous TCP port scanner built with C++ and Boost.Asio for high-concurrency network reconnaissance.

*This is a quick overview — security theory, architecture, and full walkthroughs are in the [learn modules](https://www.google.com/search?q=%23learn).*

> *Developed by [@jenishraiyani5-oss*](https://github.com/jenishraiyani5-oss)

---

## Features

* **Asynchronous I/O:** High-concurrency TCP port scanning using `Boost.Asio`.
* **Flexible Targets:** Supports single port, multiple port, and full range (`1-65535`) scans.
* **Concurrency Control:** Fine-tune worker threads/tasks to balance speed against network load.
* **Timeout Management:** Configurable connection timeouts to handle filtered or quiet ports.
* **Terminal Output:** Displays real-time status (`OPEN`, `CLOSED`, `FILTERED`) for target ports.

---

## Prerequisites

* **C++ Compiler:** C++20 compliant compiler (`GCC`, `Clang`, or `MSVC`)
* **CMake:** Version 3.31 or higher
* **Libraries:** Boost (`Boost.Asio`)

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

# Run a scan
./simplePortScanner --target 192.168.1.1 --ports 1-1024

```
---

## Usage Examples

```bash
# Scan specific ports with 200 concurrent threads/tasks
./simplePortScanner --target 10.0.0.1 --ports 22,80,443 --concurrency 200

# Full range port scan with custom 500ms timeout
./simplePortScanner --target 172.16.0.5 --ports 1-65535 --timeout 500

```

---

## Learn

Step-by-step guides covering security theory, network mechanics, and implementation walkthroughs.

| Module | Topic 

| :--- | :--- |

| [00 - Overview](./LEARN.md) | Prerequisites and initial setup |
| [01 - Concepts](./LEARN.md) | Security theory and scan mechanics |
| [02 - Architecture](./LEARN.md) | System design & asynchronous data flow |
| [03 - Implementation](./LEARN.md) | Code walkthrough (C++ & Boost.Asio) |
| [04 - Challenges](./LEARN.md) | Exercises and code extension ideas |
---

## License

Distributed under the [AGPLv3 License](https://www.google.com/search?q=https://www.gnu.org/licenses/agpl-3.0).

```

```
