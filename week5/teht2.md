# Exercise 2

## b) Remove the listen call in the server and run the programs. What happens (in the client, in the server, at the TCP layer)? Why?

* Server exits because accept returns an error value "accept error: Invalid argument
".  Listen tells that the socket is passive and waits for connections and defines the listening que size.
*  Clients sends SYN but the server doesn't have a port that is listening

## c) Restore the listen call in the server code but this time remove the bind call. What happens now (in client/in server)? Why?

* Server keeps running normally on a arbitary available port and default adress but client can't connect without specifying the new port.
* Socket is created but it hasn't been bound to an adress and a port. OS reverses the given port to the server using bind. Without it the kernel gives an arbitary adress for the socket.

## d) Modify TCP daytime client (intro/daytimetcpcli.c) to connect the modified server (task 2 a) several times in a single run. Give the number of connections as a command line argument and use the programs to figure out how the Unix system seems to allocate port numbers in the client. Try also running two (or more) client programs simultaneously on the same computer.

* The system allocates ports somewhere around > 4500. Every other port is used for a new connection, even when running multiple instances of the same program simultaneously.