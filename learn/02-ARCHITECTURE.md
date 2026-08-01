### Key Updates Made

* **CLI Standardizations:** Aligned the command line arguments (`--target` and `--ports`) throughout the walkthroughs and descriptions to match your main CLI design.


* **Code & Path References:** Updated method signatures, source file locations, and line-by-line data flow explanations to fit your codebase.


* **Formatting:** Cleaned up component diagrams, system layers, and code snippets for enhanced readability.



---

```markdown
# System Architecture

This document breaks down how the port scanner is designed and why asynchronous I/O with concurrent workers provides both speed and clarity[cite: 5].

---

## High-Level Architecture


```

┌─────────────────────────────────────┐
│      Command Line Interface         │
│   (Boost.Program_Options Parser)    │
└──────────────┬──────────────────────┘
│
▼
┌─────────────────────────────────────┐
│        PortScanner Object           │
│   - Configuration Management        │
│   - Work Queue (ports to scan)      │
│   - Thread/Concurrency Control      │
└──────────────┬──────────────────────┘
│
▼
┌─────────────────────────────────────┐
│       Boost.Asio io_context         │
│    (Event Loop / Async Runtime)     │
└──────────────┬──────────────────────┘
│
┌───────┴───────┐
▼               ▼
┌─────────────┐  ┌─────────────┐
│   Socket    │  │    Timer    │
│   (TCP      │  │  (Timeout   │
│ Connection) │  │  Detection) │
└─────────────┘  └─────────────┘
│               │
└───────┬───────┘
▼
┌───────────────┐
│    Target     │
│  Host:Port    │
└───────────────┘

```

### Component Breakdown

**Command Line Interface (`main.cpp`)**[cite: 5]
- **Purpose:** Parse user arguments and initialize scanner settings[cite: 5].
- **Responsibilities:** Validate input parameters, set default fallbacks, display usage guides[cite: 5].
- **Interfaces:** Configures the `PortScanner` instance and invokes execution methods[cite: 5].

**PortScanner Controller (`PortScanner` class)**[cite: 5]
- **Purpose:** Orchestrate the scanning pipeline and manage concurrency limits[cite: 5].
- **Responsibilities:** Maintain target port queue, enforce concurrency boundaries, track status counts (`OPEN`, `CLOSED`, `FILTERED`), format output[cite: 5].
- **Interfaces:** Exposes `set_options()`, `start()`, and `run()` methods[cite: 5].

**Boost.Asio `io_context`**[cite: 5]
- **Purpose:** Event loop that drives asynchronous I/O operations[cite: 5].
- **Responsibilities:** Schedule socket connection attempts and timer callbacks, dispatch completion handlers, manage thread execution strands[cite: 5].

**Socket and Timer Pairs**[cite: 5]
- **Purpose:** Every target port probe pairs an active TCP socket with a deadline timer[cite: 5].
- **Responsibilities:** Sockets attempt active TCP connections; timers race sockets to detect filtered ports when network packets are silently dropped[cite: 5].

---

## Data Flow

### Primary Scanning Flow

Step-by-step walkthrough of what happens when executing:
`./simplePortScanner --target 192.168.1.1 --ports 80-443 --concurrency 100 --timeout 2`


```

1. main.cpp → Parse CLI flags
Extracts target host (192.168.1.1), port range (80-443), concurrency limit (100), and timeout value (2s).
2. main.cpp → Initialize PortScanner
Invokes set_options() to resolve targets to IP endpoints.
3. PortScanner.cpp → setup_queue()
Populates port queue with targets (e.g., 80, 81, 82 ... 443).
4. PortScanner.cpp → start()
Dispatches initial scan tasks up to the concurrency limit (MAX_THREADS) to the io_context via a strand.
5. main.cpp → run()
Invokes io_context.run(), which blocks until all queued operations complete.
6. PortScanner.cpp → scan() (Executed concurrently)
Pops a target port, instantiates socket and timer objects, and initiates a race:
* IF timer expires first:
-> State: FILTERED
-> Print result, call scan() to process next queued port.
* IF connection succeeds:
-> State: OPEN
-> Execute async_read_some to attempt banner grabbing.
-> Print result, call scan() to process next queued port.
* IF connection is refused/fails:
-> State: CLOSED
-> Print result, call scan() to process next queued port.


7. Queue Exhausted -> Event loop terminates -> Final statistics displayed.

```

---

## Design Patterns

### 1. Asynchronous I/O with Completion Handlers
Non-blocking network operations return control immediately[cite: 5]. When socket states change or timers expire, registered completion handlers execute[cite: 5].

```cpp
// Non-blocking TCP connection request with explicit strand execution
socket->async_connect(endpoint, boost::asio::bind_executor(strand, 
    [this, socket, timer, port, complete](boost::system::error_code ec) {
        if (*complete) return; // Timer already won the race
        *complete = true;
        timer->cancel();        // Stop deadline timer
        
        if (!ec) {
            // Target port OPEN
            async_read_some(...); // Banner grab
        } else {
            // Target port CLOSED
            print_result(...);
        }
        scan(); // Fetch next port recursively
    }
));

```

### 2. Work Queue with Fixed Concurrency

A FIFO queue (`std::queue<uint16_t>`) manages all remaining port targets. Worker routines pull from this queue while enforcing an upper limit (`MAX_THREADS` / concurrency setting) to avoid system resource exhaustion or OS socket descriptor exhaustion.

### 3. Strands for Handler Synchronization

`boost::asio::strand` guarantees that completion handlers execute sequentially even across multithreaded runtimes. This prevents race conditions when updating shared counters and output streams without requiring manual mutex locking.

---

## Layer Separation

```
┌────────────────────────────────────┐
│    Presentation Layer              │
│    - CLI parsing (main.cpp)        │
│    - Terminal output formatting    │
└────────────────────────────────────┘
                 ↓
┌────────────────────────────────────┐
│    Business Logic Layer            │
│    - PortScanner class             │
│    - Scan algorithms & state       │
└────────────────────────────────────┘
                 ↓
┌────────────────────────────────────┐
│    I/O Infrastructure Layer        │
│    - Boost.Asio event loop         │
│    - Sockets & Timers              │
└────────────────────────────────────┘

```

---

## Technical Trade-Offs

### Full TCP Connect vs. SYN Stealth Scanning

* **Choice:** Full TCP Connect Scan (`async_connect`).


* **Reasoning:** Full TCP connection scans work on standard user privileges without requiring root/Administrator access for raw socket creation.


* **Trade-Off:** Connect scans are logged by target services and are slightly slower than half-open SYN scans.



### Timeout-Based Filtering vs. ICMP Packet Analysis

* **Choice:** Deadline timer-based detection.


* **Reasoning:** Distinguishes dropped packets (`FILTERED`) from explicit socket refusals (`CLOSED`). Works across standard firewall rule sets where ICMP unreachable packets are discarded.



```

```