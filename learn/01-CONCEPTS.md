**File & Code Alignment:** Updated source code references (`src/PortScanner.cpp`) and method logic to match your project structure.


* **CLI Flags:** Standardized target and port scan command flags across code explanations (`--target` and `--ports`).


* **Formatting:** Cleaned up ASCII sequence diagrams, Markdown tables, and section anchors for better scannability.



---

```markdown
# Core Security Concepts

This document explains the security concepts you'll encounter while building and using this project[cite: 4]. These are practical concepts used daily in penetration testing, incident response, and network security[cite: 4].

---

## 1. TCP Port Scanning

### What It Is
Port scanning is the process of probing a target host to determine which TCP or UDP ports are accepting connections[cite: 4]. Each port number (`0-65535`) can have a service listening on it[cite: 4]. Scanning reveals what software is running on a system without requiring authentication[cite: 4].

Ports `1-1023` are "well-known" ports assigned to standard services (e.g., HTTP on port 80, SSH on port 22), while higher ports can run custom applications[cite: 4].

### Why It Matters
Port scanning is **reconnaissance**, the first phase of the cyber attack lifecycle[cite: 4]. Every penetration test, vulnerability assessment, and real-world attack starts by mapping what is accessible[cite: 4].

* **2017 Equifax Breach:** Started with reconnaissance that identified an exposed Apache Struts instance on port 8080[cite: 4].
* **2016 Mirai Botnet:** Malware scanned the internet for IoT devices with exposed Telnet (port 23), compromising hundreds of thousands of routers and cameras[cite: 4].

### How It Works
TCP port scanning leverages the standard TCP three-way handshake[cite: 4]:


```

Scanner                Target
|                     |
|------ SYN --------->|  (Initiate connection)
|<--- SYN-ACK --------|  (OPEN: Service listening)
|------ RST --------->|  (Scanner aborts handshake)

```


```

Scanner                Target
|                     |
|------ SYN --------->|  (Initiate connection)
|<---- RST -----------|  (CLOSED: No service listening)

```


```

Scanner                Target
|                     |
|------ SYN --------->|  (Initiate connection)
|     (Silence)       |  (FILTERED: Firewall dropped packet)

```

---

## 2. Port States and Their Meaning

From a scanner's perspective, target ports are classified into three core states[cite: 4]:

| State | Description | Response Mechanism |
| :--- | :--- | :--- |
| **`OPEN`** | A service is actively accepting connections[cite: 4]. | Target responds with `SYN-ACK`[cite: 4]. |
| **`CLOSED`** | Host is reachable, but no service is listening[cite: 4]. | Target responds with `RST` (Reset)[cite: 4]. |
| **`FILTERED`** | A firewall or filter is blocking packets[cite: 4]. | No response received before timeout[cite: 4]. |

### Scanner Logic Implementation
In this project, port states are determined using asynchronous handlers (`src/PortScanner.cpp`)[cite: 4]:

```cpp
// OPEN State: Successful TCP handshake
socket->async_connect(endpoint, [](boost::system::error_code ec) {
    if (!ec) {
        // State: OPEN -> Attempt Banner Grab
    }
});

```

```cpp
// CLOSED State: Host actively refuses connection (RST)
else if (ec == boost::asio::error::connection_refused) {
    // State: CLOSED
}

```

```cpp
// FILTERED State: Connection attempt exceeds timeout
timer->async_wait([](boost::system::error_code ec) {
    if (!ec && !is_complete) {
        // State: FILTERED
    }
});

```

---

## 3. Banner Grabbing

### What It Is

Banner grabbing involves reading the initial response string sent by a service upon establishing a TCP connection. Many servers announce their software and version details immediately (e.g., `SSH-2.0-OpenSSH_8.2p1` or `Server: Apache/2.4.41`).

### Why It Matters

Service banners expose precise software versions:

* **Vulnerability Identification:** Allows security auditors to match service versions against CVE databases.


* **Attack Surface Mapping:** Helps identify outdated or unpatched software across subnets.



### Implementation Details

Once a TCP socket successfully connects, the scanner attempts a non-blocking read operation:

```cpp
// Attempt to capture initial bytes sent by server
socket->async_read_some(boost::asio::buffer(*buf),
    [](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec && bytes_transferred > 0) {
            banner->assign(buf->data(), bytes_transferred);
        }
    });

```

---

## 4. Security Framework Alignments

### MITRE ATT&CK Matrix

* **[T1046 - Network Service Discovery](https://attack.mitre.org/techniques/T1046/):** Active port scanning to discover running services.


* **[T1595.001 - Active Scanning: IP Blocks](https://attack.mitre.org/techniques/T1595/001/):** Automated scanning across wide network scopes.



### OWASP Top 10

* **A05:2021 – Security Misconfiguration:** Unnecessary ports exposed publicly increase target attack surface.


* **A01:2021 – Broken Access Control:** Internal services exposed without proper access boundaries.



---

## 5. Defense & Mitigation Strategies

1. **Minimize Exposed Services:** Disable unnecessary services and bind management endpoints strictly to `127.0.0.1`.


2. **Default-Deny Firewall Rules:** Configure firewalls to drop unexpected inbound traffic silently (`FILTERED` state).


3. **Banner Suppression:** Hide detailed version headers in web servers and SSH configurations (e.g., `server_tokens off` in Nginx, `DebianBanner no` in OpenSSH).


4. **Intrusion Detection (IDS):** Monitor for high-volume `SYN` packets across sequential ports to detect scanning activity.



---

## Reference Resources

* **[RFC 793 - Transmission Control Protocol (TCP)](https://datatracker.ietf.org/doc/html/rfc793)**

* **[Nmap Network Scanning (Gordon Lyon)](https://nmap.org/book/)**

* **[IANA Service Name & Port Registry](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml)**


```

```