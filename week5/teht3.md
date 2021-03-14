# Exercise 3
---
## a) Test your programs and then modify the client to close the read direction and the server to close the write direction of the connection immediately after the connection has been established (use shutdown() call). Does the program operation change? Why/Why not? What happens at the TCP layer?

* Server and client continue to work normally. Because both sockets do not use the closed direction
* The socket wont handle read/writes. Tcp should send HALF close from the server to the client but the client is able to send more data because it side of the connection is still open.

## b) Next modify the server to close the read direction of the connection (and to stop reading, of course) either immediately or after receiving some data. Make sure that the client, however continues sending data. What happens now in the client / in the server / at the TCP layer?

* The server accepts the clients connection but of course doesn't read anything. Client writes curiosly doesn't return -1 but continues working
* Socket wont handle reads and receive buffer is discarded. Received data is acknowledged so the clients write doesn't return -1 but a 0 silently discarded

## c) Modify the server to close both the read and write direction of the connection either immediately or after receiving some data. Again, make sure that the client, however, continues sending data. What happens now in the client/ in the server / at the TCP layer? How should this kind of situation be taken into account in the client code?
* The server accepts the clients connection but of course doesn't read anything. Client exits after two write attemps.
* Client has received HALF close FIN from the server and tries to write but write=0 so also it also send FIN and the connection closes.
* Handle the broken pipe signal and handle possible write error
