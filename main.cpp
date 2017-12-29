//R. Ramanuj, 2017. BT Node Status Reporting.

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Node.h"
using namespace std;

//prototypes
string determine_status(Node sender);
void update_node(Node sender, char element, vector<Node>& vectorOfNodes, string status);
void add_friend_node(Node sender, vector<Node>& vectorOfNodes);
void analyse_node(Node node, vector<Node>& vectorOfNodes);
void parse_input(string x, vector<Node>& vectorOfNodes);


int main(int argc, char* argv[]) {
/*	cout << "argc = " << argc << endl; //argc is the number of elements argv[] contains
									   //if you enter more text into command arguments, argc will get larger
	cout << "argv[0] = " << argv[0] << endl; //argv[0] is the file name and location
	cout << "argv[1] = " << argv[1] << endl; //argv[1] is the text entered into the text field
	*/
	vector<Node> vectorOfNodes;
	string status, name;
	long long int time1, time2;

	if (argc > 1) {
		cout << "argv[1] = " << argv[1] << endl;
	}
	else {
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
		cout << " " << vectorOfNodes[i].get_message() << "\n";
	}

	cout << "UNKOWN Values\n";
	//in order to determine ambuigity, this function looks for patterns where the generated time is <= 50,
	//and the received time is out of sync. In this case, the node will be marked as unknown.
	cout << "The notifications here are set to UNKOWN as there is a discrepency between the time the node was generated and the time it was received to the monitoring system.";
	system("Pause");
	return 0;
}

//this interprets the message on the sender node to action onto the friend
string determine_status(Node sender) {
	string status;

		if (sender.get_message() == "LOST")	//if the sender says the node is lost
		{
			status = ("DEAD");	//we set the friend node status to dead
		}
		if (sender.get_message() == "FOUND")	//if the sender says the node is lost
		{
			status = ("ALIVE");	//we set the friend node status to dead
		}
	return status;
}


//this function updates a single node within the vector
void update_node(Node sender, char element, vector<Node>& vectorOfNodes, string status) {
	if (vectorOfNodes[element].get_generated_time() < sender.get_generated_time()) 
	{
		//updates on node only performed if the message is newer than the message currently stored
		//conventially, if the generated time is later the received time should follow suit. 
		//this clause is here to catch disparties between the received to monitoring and generated time.
		//if something is flagged here, we examine the differences, if the generated time < 50 ms - we will flag the item as unknown.
		if (vectorOfNodes[element].get_received_time() > sender.get_received_time()) {
			//a delta of <50 will indicate that the node events were generated in successsion, however travel time to the monitor was delayed connected which makes the status of the node ambigious.
			//
			if (vectorOfNodes[element].get_generated_time() - sender.get_generated_time() <= 50)
			{
				int x = vectorOfNodes[element].get_generated_time() - sender.get_generated_time();
				status = "UNKNOWN";
			}
		}
		vectorOfNodes[element].set_status(status);
		vectorOfNodes[element].set_generated_time(sender.get_generated_time()); //set this to the generated time from the sender 
		vectorOfNodes[element].set_received_time(sender.get_received_time());  //set friend received time to sender received time
		vectorOfNodes[element].set_message(sender.get_name() + " " + sender.get_message() + " " + sender.get_friend_name());	//set to update + friend name
		vectorOfNodes[element].set_friend_name(sender.get_name()); //set to communication node	
	}
	}

//this function is called when the system needs to add a new FRIEND node since it's not correctly in the vector of nodes.
void add_friend_node(Node sender, vector<Node>& vectorOfNodes)
{
	string status = determine_status(sender);
	Node friendNode(sender.get_received_time(), sender.get_generated_time(), sender.get_friend_name(), sender.get_name() + " " + sender.get_message() + " " + sender.get_friend_name(), sender.get_name());
	friendNode.set_status(status);
	//create a new instance of a node for the friend using the details supplied the sender node
	vectorOfNodes.push_back(friendNode);
}

//the purpose of this function is seperate the update into two areas:
// 1. Any messages that may influence another node (friend node)
// 2. Set the appropriate status from the communication node. (If a node has sent a message, it will set its own status to alive.)
void analyse_node(Node node, vector<Node>& vectorOfNodes) {
	char senderNo, friendNo;
	bool nodeExists = false;
	bool friendExists = false;

	//the purpose of this loop is to find out if any of the node/friend nodes currently exist in the vector
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
		{	//run the function to update an existing node
			update_node(node, senderNo, vectorOfNodes, "ALIVE");
		}
	}
	else if (!nodeExists) {	//if node isn't currently on the vector
		node.set_status("ALIVE");	//set the status to alive (implicit since it's sending)
		vectorOfNodes.push_back(node);	//put back to the vector
	}
	if (friendExists) 
	{	//if the node which the sender node is communicating to exists

		string status = determine_status(node);
		update_node(node, friendNo, vectorOfNodes, status);

	}
	else if (node.get_friend_name() != "")
	{
		add_friend_node(node, vectorOfNodes);
	}
}

//this function parses the input.txt file into strings which are then used to create node instances from the Node class
void parse_input(string x, vector<Node>& vectorOfNodes) {
	string temp; 
	vector <string> input; 	//store input parses in a vector
	stringstream s(x);
	long long int received, generated; //long int values to store received time & generated time 

	//iteration to 
	while (s >> temp)
		input.push_back(temp);

	stringstream r(input[0]);
	stringstream g(input[1]);


	r >> received;
	g >> generated;

	if (input[3] == "HELLO")
	{
		Node newNode(received, generated, input[2], input[3], "");
		analyse_node(newNode, vectorOfNodes);
	}
	else
	{
		Node newNode(received, generated, input[2], input[3], input[4]);
		analyse_node(newNode, vectorOfNodes);
	}
}