Read Me


Run
This program has been tested using G++ 6.3.
1. Compile the .cpp files within command line using the following command:
gcc main.cpp, node.cpp
2. run the .exe along with "input.txt"


Logic:
The program reads each line of data within the input file and searches for discussion of any nodes. If a node is detected, an instance
of the 'node' class is created for the node, and the instance is then added to a vector of nodes.
Each line read in by the system may contain 2 nodes, I refer to them as a 'sender node', and a 'friend node'
the sender node refers to the node which sent the notification. If the system detects a sender node, the sender node status is implicity
set to alive (since it must be alive to send the message). 
Adding a new node.
Friend nodes refer to nodes which are being referenced by the sender node. E.G.
1508405807378 1508405807387 luke LOST vader - In this example Luke is the sender, and Vader is the friend.
If a friend node is detected on a line, it is also added to the vector (providing it doesn't already exist), however the status is dependant on the notification.
Updation of nodes:
If nodes already exist in the vector, then they are updated with the latest notification. Updates are strictly confined to the latest generated reference, the received
time is irrelivant in this context. If a notification is read into the system, and a later generated timestamp notification is already existant within the node, the 
notification is ignored. 

If however, the generated time is indeed newer, there is another clause in regards to ambuigity. If the generated time between the last notification is <=50 ms, this indicates
the notifications were generated in sequence. If however the received times for the respective notifications are in a different order (lets say a delta of >100) - this will indicate 
the notifications arrived into the system at the wrong order, and the node is set to UNKNOWN.


Assumptions
