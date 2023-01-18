# How to run
## Client code
gcc client.c -o client
./client 127.0.0.1 9999 (You can put any port greater than 1023)

## server code

gcc server.c -o server
./server 9999 (put any port greater than 1023 but should be same as client code)

also start server first then client.
