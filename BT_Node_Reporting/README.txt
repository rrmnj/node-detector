Rasesh Ramanuj – BT Code Test

Files within directory:
main.cpp, Node.cpp, Node.h, input.txt, README.txt

Run
This program has been tested using compiler g++ 6.3.
1. Compile the .cpp files within command line using the following command:
g++ main.cpp node.cpp
2. run the .exe along with the input file with:
a input.txt

Logic:
The program reads each line of data in the input file and searches for the mention of any nodes. If a node is detected, an instance
of the 'node' class is created for the node, and the instance is then added to a vector of nodes.
Each notification line read in by the system may contain 2 nodes, I refer to them as a 'sender node', and a 'friend node' (respectively on the notification)
the sender node refers to the node which sent the notification. If the system detects a sender node, the sender node status is implicitly
set to alive (since it must be alive to send the message). 
Adding a new node.
Friend nodes refer to nodes which are being referenced by the sender node. E.G.
1508405807378 1508405807387 luke LOST vader - In this example ‘luke’ is the sender, and ‘vader’ is the friend.
If a friend node is detected on a line, a node instance is created for it, and it is added to the vector (providing it doesn't already exist), 
however the status is dependent on the notification.
Updating nodes:
If node instances already exist in the vector, then they are updated with the latest notification (latest refers to later generated time). Updates are strictly confined to the latest generated reference, the received
time is irrelevant in this regard. If a notification is read into the system, and a later generated timestamp notification is already existent within the node, the 
notification is ignored. 

If however, the generated time is later, there is another clause in regards to ambiguity. If the generated time between the last notification is <=50 ms, 
this indicates the notifications were generated in sequence. If, however the received times for the respective notifications were in a different order (e.g. a negative delta) - this will indicate 
the notifications arrived into the system in a different sequence then they were created, and the node is set to UNKNOWN.

Node Ambiguity:
To clarify the systems definition of an UNKNOWN node take the following input data:

1508405807242 1508405807141 vader HELLO
1508405807340 1508405807350 luke HELLO
1508405807378 1508405807387 luke LOST vader
1508405807467 1508405807479 luke FOUND r2d2
1508405807468 1508405807480 luke LOST leia
1508405807512 1508405807400 vader LOST luke
1508405807560 1508405807504 vader HELLO

This delivers the following output:

vader ALIVE 1508405807560 vader HELLO
luke ALIVE 1508405807468 luke LOST leia
r2d2 ALIVE 1508405807467 luke FOUND r2d2
leia DEAD 1508405807468 luke LOST leia

No notifications in this test case were deemed ambiguous. I'm going to add the following 2:
1508405807812 1508405807600 han LOST solo
1508405807360 1508405807620 han FOUND solo

This now outputs:
vader ALIVE 1508405807560 vader HELLO
luke ALIVE 1508405807468 luke LOST leia
r2d2 ALIVE 1508405807467 luke FOUND r2d2
leia DEAD 1508405807468 luke LOST leia
han ALIVE 1508405807360 han FOUND solo
solo UNKNOWN 1508405807360 han FOUND solo
TIME DISPARITY: solo GENERATED TIME:1508405807600 1508405807620 delta =20
RECEIVED TIME: 1508405807812 1508405807360 delta =-452

The status of node 'solo' is set to UNKNOWN. Whenever a node has a status of unknown, it is accompanied by a reason.
The system has detected a time disparity between the generated time & the received time.
If the delta between the generated times is less than 50 ms, this indicates the nodes were generated in succession. 
However, the received time delta is a negative which means it wasn’t received to the monitoring system in the same order 
it was sent, so the status of the node is unknown. 
Notice the sender node however is marked as alive, this is intentional as the sender must be alive to send the notification. 
