#include "Node.h"

Node::Node(const long long int& received_time, const long long int& generated_time, const string& name, const string& notification, const string& friendName) : name_(name), notification_(notification),received_time_(received_time),generated_time_(generated_time), friend_name_(friendName){}


const string Node::get_name() {
	return name_;
}

const string Node::get_status() {
	return status_;
}
const string Node::get_notification() {
	return notification_;
}

const string Node::get_friend_name() {
	return friend_name_;
}

const long long int Node::get_generated_time() {
	return generated_time_;
}

const long long int Node::get_received_time() {
	return received_time_;
}

const string Node::get_ambigious_reason() {
	return ambuigity_reason_;
}
//The 'friend node' refers to the node the sender may be communicating with in his notification. If the message is a 'hello' message, then the friend node
void Node::set_friend_name(const string friendName) {
	friend_name_ = friendName;
}

void Node::set_ambigious_flag(const string reason) {
	ambuigity_reason_ = reason;
}

void Node::set_notification(const string notification) {
	notification_ = notification;
}

void Node::set_received_time(const long long int time) {
	received_time_ = time;
}

void Node::set_generated_time(const long long int time) {
	generated_time_ = time;
}
void Node::set_name(const string Name) {
	name_ = Name;
}

void Node::set_status(const string status) {
	status_ = status;
}
