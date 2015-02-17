/*
 * rosbag_controller_node.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: Davide A. Cucci
 */

#include <string>
#include <iostream>

#include <spawner.h>

#include <ros/ros.h>

#include <roscpp/Empty.h>

bool _is_bag_paused = true;
Spawner *rosbag;

bool pause(roscpp::Empty::Request &req, roscpp::Empty::Response &answ) {
  if (!_is_bag_paused) {
    rosbag->stdin << ' ';
    rosbag->stdin.flush();

    _is_bag_paused = true;
  }

  ROS_INFO("bag paused");
}

bool unpause(roscpp::Empty::Request &req, roscpp::Empty::Response &answ) {
  if (_is_bag_paused) {
    rosbag->stdin << ' ';
    rosbag->stdin.flush();

    _is_bag_paused = false;
  }

  ROS_INFO("playback resumed");
}

int main(int argc, char * argv[]) {

  ros::init(argc, argv, "rosbag_controller_node");

  ros::NodeHandle n;

  // --- construct the argument vector and spawn rosbag

  const char* rosbag_args[6];

  rosbag_args[0] = argv[1];
  rosbag_args[1] = "play";
  rosbag_args[2] = "--clock";
  rosbag_args[3] = "--pause";
  rosbag_args[4] = argv[2];
  rosbag_args[5] = (const char *) NULL;

  rosbag = new Spawner(rosbag_args);

  // wait 1 sec and see if the process is alive before advertising services

  sleep(1.0);

  if (!rosbag->is_alive()) {
    ROS_FATAL("rosbag has died for some reason");

    return 1;
  }

  ROS_INFO("advertising services... ");

  // --- advertise pause and unpause services

  ros::ServiceServer pause_srv = n.advertiseService("pause_bag", pause);
  ros::ServiceServer unpause_srv = n.advertiseService("unpause_bag", unpause);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  // --- consume output from rosbag;

  char buf[1024];

  ros::Rate r(10);

  while (ros::ok()) {
    r.sleep();
    rosbag->stdout.readsome(buf, 1024);
  }

  ros::waitForShutdown();

  return 0;
}
