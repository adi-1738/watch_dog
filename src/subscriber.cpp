#include "ros/ros.h"
#include "std_msgs/String.h"

void listener(const std_msgs::String::ConstPtr& msg)//this function runs whenever a new message is published on the topic that this subscriber is listening to, the message is passed as an argument to this function and we can access its data using msg->data
{
    ROS_INFO("I heard: %s", msg->data.c_str());//ros_info is a logging function that prints the message to the terminal, msg->data.c_str() converts the string data from the message into a C-style string for printing onto the terminal
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "subscriber_node");
    ros::NodeHandle n;
    ros::Subscriber subscriber_object = n.subscribe("publisher_topic", 100, listener);//creating a subscriber object that listens to the  topic and call the listener function whenever a new message is received on that topic
    ros::spin();//to wait for incoming messages or subscriber exits .
    return 0;
}