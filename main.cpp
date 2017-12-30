//R. Ramanuj, 2017. BT Node Status Reporting.

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Node.h"
using namespace std;

//function prototypes
string determine_status(Node sender);
void update_node(Node sender, char element, vector<Node>& vectorOfNodes, string status);
void add_friend_node(Node sender, vector<Node>& vectorOfNodes);
void analyse_node(Node node, vector<Node>& vectorOfNodes);
void parse_input(string x, vector<Node>& vectorOfNodes);


int main(int argc, char* argv[]) {
	vector<Node> vectorOfNodes;
	string status, name;
	long long int time1, time2;

	//if no file name is entered, exit program
	if (argc < 1){
		cout << "No file name entered. Exiting...";
		return -1;
	}

	ifstream infile(argv[1]); //open the file

	if (infile.is_open() && infile.good()) {
		string line = ""; //parse value
		while (getline(infile, line)) {
			parse_input(line, vectorOfNodes);
		}
	}

	else {
		cout << "Failed to open file..";
	}

	for (int i = 0; i < vectorOfNodes.size(); ++i) {
		cout << vectorOfNodes[i].get_name() + " " << vectorOfNodes[i].get_status() << + " " << vectorOfNodes[i].get_received_time();
		cout << " " << vectorOfNodes[i].get_notification() << "\n";
	}

	//in order to determine ambuigity, this function looks for patterns where the generated time is <= 50,
	//and the received time is out of sync. In this case, the node will be marked as unknown.

	for (int i = 0; i < vectorOfNodes.size(); ++i)
	{
		if (vectorOfNodes[i].get_status() == "UNKNOWN")
		{
			cout << "TIME DISPARITY: " << vectorOfNodes[i].get_name() << " " << vectorOfNodes[i].get_ambigious_reason() << "\n"; //displays further details about ambigious nodes (why they were flagged)
		}
	}
	system("Pause"); //remove
	return 0;
}

//this interprets the message on the sender node to action onto the friend
string determine_status(Node sender) {
	string status;

		if (sender.get_notification() == "LOST")	//if the sender says the node is lost
		{
			status = ("DEAD");	//we set the friend node status to dead
		}
		if (sender.get_notification() == "FOUND")	//if the sender says the node is lost
		{
			status = ("ALIVE");	//we set the friend node status to dead
		}
	return status;
}


//this function updates a single node within the vector
void update_node(Node sender, char element, vector<Node>& vectorOfNodes, string status) {
	if (vectorOfNodes[element].get_generated_time() < sender.get_generated_time()) 
	{
		//updates on node only performed if the notification is newer than the notification currently stored
		//theoretically, if the generated time is later, then the received time would also be later. However we know this may be untrue as notifications aren't received sequentially
		//this clause is here to catch disparties between the received and generated time.
		//if something is flagged here, we examine the differences, if the generated time < 50 ms - we will flag the node status as unknown.

		if (vectorOfNodes[element].get_received_time() > sender.get_received_time()) {	//notice we're using a greater than > sign to compare these numbers. If the received time is greater than the sent time (yet the generated time suggests
																						//the notification is newer, we can conclude they arrived to the monitoring system in the wrong order.
			//a delta of <50 will indicate that the node events were generated in successsion, however travel time to the monitor was delayed connected which makes the status of the node ambigious.
			
			long int gen_delta = (sender.get_generated_time() - vectorOfNodes[element].get_generated_time());	//delta between generated times	- If this is <50 we know they were sent sequentially	
			long int rec_delta = (sender.get_received_time() - vectorOfNodes[element].get_received_time());	

			if ((gen_delta <= 50) & (sender.get_name() != vectorOfNodes[element].get_name()))
			{
				status = "UNKNOWN";
				string reason = "GENERATED TIME:" + to_string(vectorOfNodes[element].get_generated_time()) + " " + to_string(sender.get_generated_time()) + " delta =" +
				to_string(gen_delta) + "\nRECEIVED TIME: " + to_string(vectorOfNodes[element].get_received_time()) + " " + to_string(sender.get_received_time()) + " delta =" + to_string(rec_delta);	//add reason code to display in main
				vectorOfNodes[element].set_ambigious_flag(reason);
			}
		}
		vectorOfNodes[element].set_status(status);
		vectorOfNodes[element].set_generated_time(sender.get_generated_time()); //set this to the generated time from the sender 
		vectorOfNodes[element].set_received_time(sender.get_received_time());  //set friend received time to sender received time
		vectorOfNodes[element].set_notification(sender.get_name() + " " + sender.get_notification() + " " + sender.get_friend_name());	//set to update + friend name
		vectorOfNodes[element].set_friend_name(sender.get_name()); //set to communication node	
	}
}

//this function is called when the system needs to add a new FRIEND node (the node the sender is referencing) since it's not correctly in the vector of nodes.
void add_friend_node(Node sender, vector<Node>& vectorOfNodes)
{
	string status = determine_status(sender);
	Node friendNode(sender.get_received_time(), sender.get_generated_time(), 
	sender.get_friend_name(), sender.get_name() + " " + sender.get_notification() + " " + sender.get_friend_name(), sender.get_name());

	friendNode.set_status(status);
	//create a new instance of a node for the friend using the details supplied the sender node
	vectorOfNodes.push_back(friendNode); //add the friend node to the array
}

//the purpose of this function is seperate the notification into two areas:
// 1. Any messages that may influence another node (friend node)
// 2. Update the sender node 'status' (If a node is a sender, it will set its own status to alive.)
void analyse_node(Node node, vector<Node>& vectorOfNodes) {
	char senderNo, friendNo;
	bool nodeExists = false;
	bool friendExists = false;

	//this loops through the vector in order to check if the sender & friend node currently exist
	for (int i = 0; i < vectorOfNodes.size(); ++i)
	{
		if (vectorOfNodes[i].get_name() == node.get_name()) {
			nodeExists = true;
			senderNo = i;
		}
		if (vectorOfNodes[i].get_name() == node.get_friend_name() && node.get_friend_name() != "")
		{
			friendExists = true;
			friendNo = i;
		}
	}
	if (nodeExists) //if node is currently stored on the vector
	{
		{	//update existing node
			update_node(node, senderNo, vectorOfNodes, "ALIVE");
		}
	}
	else if (!nodeExists) {	//if node isn't currently on the vector
		node.set_status("ALIVE");	//set the status to alive (implicit since it's sending)
		vectorOfNodes.push_back(node);	//add to the vector
	}
	if (friendExists) 
	{	//if the friend node exists
		string status = determine_status(node);
		update_node(node, friendNo, vectorOfNodes, status);
	}
	else if (node.get_friend_name() != "")
	{
		add_friend_node(node, vectorOfNodes); 
	}
}

//this function parses the input.txt file into strings which are then used to create node instances from the Node class
void parse_input(string line, vector<Node>& vectorOfNodes) {
	string temp;	//temporarily stores the string through >>, to be then pushed into the vector
	vector <string> input; 	//store input parses in a vector
	stringstream nextLine(line);
	long long int received, generated; //long int values to store received time & generated time 

	//iteration to add each individual string passed to the function into the vector 
	while (nextLine >> temp)
		input.push_back(temp);

	stringstream r(input[0]);
	stringstream g(input[1]);


	r >> received;		//converts the received & generated time into long ints
	g >> generated;		

	if (input[3] == "HELLO")
	{
		//this was necessary as an error would be thrown if I tried to pass input[4] whilst it was empty
		Node newNode(received, generated, input[2], input[3], "");
		analyse_node(newNode, vectorOfNodes);
	}
	else
	{
		Node newNode(received, generated, input[2], input[3], input[4]);
		analyse_node(newNode, vectorOfNodes);
	}
}