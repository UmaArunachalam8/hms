#include <ros/ros.h>
#include <hms_client/hms_msg.h>
#include <hms_client/ping_pong.h>
#include <cstdlib>
#include<cctype>
#include<string>
using namespace std;

bool check(hms_client::ping_pong::Request  &req,
         hms_client::ping_pong::Response &res)
{
  res.msg.header.stamp = ros::Time::now();
  res.health = 1;
  res.error_code = 0;
  //ROS_INFO("%s", "hello from node 2");
  //sleep(1);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "node2");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("health_check_node2", check);

  ros::Rate loop_rate(20);
  //ros::MultiThreadedSpinner spinner(2);

  while(ros::ok()) {
    ros::spinOnce();
    //spinner.spin();
    loop_rate.sleep();
  }

  return 0;
}