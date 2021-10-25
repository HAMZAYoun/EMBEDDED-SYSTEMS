# EMBEDDED-SYSTEMS
This is for interfacing rasberry-pi's (2 cards) with an arduino for sending raw data to form a closed loop system to avoid motor heating by acting on a given temperature. 
Interface is explained through a master slave approach and client server (TCP/IP protocol connected by an Ethernet link). Another camera is used with OPEN-CV platform to interface and collect data aswell.

The project objective is the development of a communication between the both
systems:
1. an interactive communication, i.e. exchange of strings ended with the character q to
terminates communication.
2. secondly, a periodic communication, every 100 ms (10 Hz) the client requests a given message
in the form of a string.
3. thirdly, a periodic communication, every 100 ms (10 Hz) the client requests an image to the
server.
