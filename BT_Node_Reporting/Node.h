#pragma once
#include <string>
using namespace std;

//This is the class stored for an individual node. Each node has a name, status (dead/alive/unknown), received & generated time, and also a friend node.
//The 'friend node' refers to the node the sender may be communicating with in his notification. If the message is a 'hello' message, then the friend node
//is blank.


class Node
{
public:
	Node(const long long int& received_time, const long long int& generated_time, const string& name, const string& notification, const string& friendName);

	//assessors
	const string get_name();
	const string get_status();
	const string get_notification();
	const string get_friend_name();
	const long long int get_generated_time();
	const long long int get_received_time();
	const string get_ambigious_reason();

	//mutators
	void set_name(const string name);
	void set_status(const string status);
	void set_notification(const string notification);
	void set_friend_name(const string friendName);
	void set_received_time(const long long int time);
	void set_generated_time(const long long int time);
	void set_ambigious_flag(const string reason);
private:
	string name_;
	string status_;
	string notification_;
	string friend_name_;
	long long int received_time_;
	long long int generated_time_;
	string ambuigity_reason_;
};

