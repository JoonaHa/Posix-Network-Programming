# Exercise 2
---
## a) Write the required code to experimentally find out the largest UDP datagram you can send. What is the result? What happens at the sending end, if the datagram is too big?

* Datagram limit is 65 507 bytes
* Sendto returns -1 with errno 90 EMSGSIZE, Message too long

## b) Try connecting a UDP socket using connect() call. What is the effect of connect() on a UDP socket? What happens in the network when connect() is called for a UDP socket?
* Connect() just sets the default adress of receiver for the UDP socket
* If there's no peer connect sendto() returns -1 and errno 111  ECONNREFUSED, Connection refused
* Closed port sends ICMP error package that connection is refused. If there's no connect() the sending application doesn't now which host send the udp package show the ICMP erro isn't delivered.