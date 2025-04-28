# Multithreaded HTTP Server

## Description
This project implements a basic multithreaded HTTP/1.1 server in C. The server supports custom routes such as:
- `/calc/add/<a>/<b>`: adds two numbers
- `/calc/mul/<a>/<b>`: multiplies two numbers
- `/calc/div/<a>/<b>`: divides two numbers (with division-by-zero handling)
- `/static/<path>`: serves static files from the `./static` directory
- `/sleep/<seconds>`: simulates delay using `sleep()` for testing pipelining

The server is capable of:
- Handling multiple clients using POSIX threads
- Parsing HTTP GET requests (no pipelining required by client)
- Returning appropriate content-type and content-length headers
- Optionally keeping connections alive via `Connection: keep-alive`

---



### Compilation
```
make
```

### Running the Server
```
./server  # Optional: specify port (default is 80)
```

---

## Directory Structure
```
.
├── main.c
├── client_handler.c/h
├── add_handler.c/h
├── mul_handler.c/h
├── div_handler.c/h
├── static_handler.c/h
├── request.c/h
├── response.c/h
├── Makefile
└── static
```

---

## Testing with Telnet

### Example Request:
```
telnet 127.0.0.1 80
```
Then type:
```
1.
GET /calc/add/4/5 HTTP/1.1 Host: localhost
Output:
    HTTP/1.1 200 OK
    Content-Type: text/plain
    Content-Length: 10
    Connection: keep-alive

    Result: 9

```
2.
GET /calc/mul/3/5 HTTP/1.1 Host: localhost
Output:
    HTTP/1.1 200 OK
    Content-Type: text/plain
    Content-Length: 11
    Connection: keep-alive

    Result: 15
```
2.
GET /calc/div/15/5 HTTP/1.1 Host: localhost
Output:
    HTTP/1.1 200 OK
    Content-Type: text/plain
    Content-Length: 10
    Connection: keep-alive

    Result: 3
```
GET /sleep/5 HTTP/1.1 Host: localhost
Output:
    HTTP/1.1 200 OK
    Content-Type: text/plain
    Content-Length: 29
    Connection: keep-alive

    Done sleeping for 5 seconds.   
---

## Testing in Browser
```
http://localhost:8080/calc/mul/3/7
http://localhost/static/images/rex.png
http://localhost:8080/static/hello.txt
http://localhost/sleep/3
```



