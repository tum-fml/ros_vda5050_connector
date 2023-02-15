/*
 * Copyright 2022 Technical University of Munich, Chair of Materials Handling,
 * Material Flow, Logistics – All Rights Reserved
 *
 * You may use, distribute and modify this code under the terms of the 3-clause
 * BSD license. You should have received a copy of that license with this file.
 * If not, please write to {kontakt.fml@ed.tum.de}.
 */

#include <math.h>
#include <ctime>
#include <iostream>
#include <string>
#include "ros/ros.h"
#include "vda5050_msgs/AGVPosition.h"
#include "vda5050_msgs/State.h"

using namespace std;

string getTimestamp() {
  time_t now;
  time(&now);
  char buf[sizeof "2011-10-08T07:07:09Z"];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
  return (buf);
}

/*
 * This is an AGV mockup to send messages to the supervisor and check if the translation works
 * It includes the following messages:
 * 		MapId
 * 		Position (x, y and theta)
 * 		BatteryCharge
 */

// creates the order msg
vda5050_msgs::State createMessage() {
  vda5050_msgs::State msg;
  msg.header.headerId = 1;
  msg.header.timestamp = getTimestamp();
  msg.header.version = "1.1";
  msg.header.manufacturer = "fml Enterprise";
  msg.header.serialNumber = "ajf894ajc";
  msg.orderId = "pass nr 3.5";
  msg.orderUpdateId = 876324;
  msg.zoneSetId = "fml hall of fame";
  msg.agvPosition.x = 0;
  msg.agvPosition.y = 0;
  msg.agvPosition.theta = 0;
  msg.agvPosition.positionInitialized = true;
  msg.agvPosition.mapId = "ae9748b3-8996-4a67-8709-cbbd40d95ea5";
  msg.batteryState.batteryCharge = 70.0;
  msg.driving = true;

  return (msg);
}

vda5050_msgs::State updateMessage(
    vda5050_msgs::State msg, float angle, float r, float mx, float my) {
  msg.agvPosition.x = r * cos(angle) + mx;
  msg.agvPosition.y = r * sin(angle) + my;
  msg.agvPosition.theta = angle;
  return (msg);
}

int main(int argc, char** argv) {
  string topicPublish = "state";
  string topicViz = "viz";
  if (argc > 1) topicPublish = argv[1];
  ros::init(argc, argv, "state_msg_mockup");
  ros::NodeHandle nh;
  ros::Rate loop_rate(10);
  ros::Publisher publisherState = nh.advertise<vda5050_msgs::State>(topicPublish, 1000);
  vda5050_msgs::State msg = createMessage();
  float mx = 30;
  float my = 30;
  float r = 10;
  cout << topicPublish << "\n";
  float angle = -M_PI;
  while (ros::ok()) {
    publisherState.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
    msg.header.headerId += 1;
    msg = updateMessage(msg, angle, r, mx, my);
    angle += 0.05;
    if (angle >= M_PI) angle = -M_PI;
  }
  return (0);
};
