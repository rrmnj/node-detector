#pragma once
#include <string>
using namespace std;
class Node
{
public:
	Node(const long long int& received_time, const long long int& generated_time, const string& name, const string& message, const string& friendName);

	//assessors
	const string get_name();
	const string get_status();
	const string get_message();
	const string get_friend_name();
	const long long int get_generated_time();
	const long long int get_received_time();

	//mutators
	void set_name(const string name);
	void set_status(const string status);
	void set_message(const string message);
	void set_friend_name(const string friendName);
	void set_received_time(const long long int time);
	void set_generated_time(const long long int time);
private:
	string name_;
	string status_;
	string message_;
	string friend_name_;
	long long int received_time_;
	long long int generated_time_;
};

