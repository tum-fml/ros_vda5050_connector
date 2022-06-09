#ifndef ACTION_DAEMON_H
#define ACTION_DAEMON_H
#include <ros/ros.h>
#include "daemon.h"
#include "vda5050_msgs/InstantActions.h"
#include <string>

using namespace std;


class ActionDaemon: public Daemon
{
	private:
	public:
	ActionDaemon(ros::NodeHandle *nh, std::string daemonName);
	void LinkPublishTopics(ros::NodeHandle *nh);
	void LinkSubscirptionTopics(ros::NodeHandle *nh);
	void PublishActions();

};

#endif

