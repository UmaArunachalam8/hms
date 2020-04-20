#include <ros/ros.h>
#include <hms_client/hms_msg.h>
#include <hms_client/ping_pong.h>
#include <sensor_msgs/PointCloud2.h>
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
  //ROS_INFO("%s", "hello from node 1");
  //sleep(1);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "node1");
  ros::NodeHandle n;
  sensor_msgs::PointCloud2 msg; 
  msg.height = 5; 
  msg.is_dense = false;
  ros::Publisher pub = n.advertise<sensor_msgs::PointCloud2>("dummy", 1000);
  ros::ServiceServer service = n.advertiseService("health_check_node1", check);

  ros::Rate loop_rate(20);
  //ros::MultiThreadedSpinner spinner(2);

  while(ros::ok()) {
    ros::spinOnce();
    //spinner.spin();
    pub.publish(msg);
    //sleep(10);
    loop_rate.sleep();
  }

  return 0;
}