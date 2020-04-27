//To include header files:
#include <ros/ros.h>
#include <hms_client/hms_msg.h>
#include <hms_client/ping_pong.h>

//Add dependencies in cmake, see node1s cmake

//Advertise service
ros::ServiceServer service;//In goal pub header
this->service = node -> advertiseService("health_check_goal_publisher", check);//In goal pub constructor

//Service call back, define prevx and prevy = 0 as global var or in main
bool check(hms_client::ping_pong::Request  &req,
         hms_client::ping_pong::Response &res)//Defined as a non class member
{
  res.msg.header.stamp = ros::Time::now();
  res.health = 1;
  res.error_code = 0;//Node is operational
  float xdiff = float(prevx - this->goal_pose.pose.position.x);
  float ydiff = float(prevy - this->goal_pose.pose.position.y);
  float dist_sq = (xdiff * xdiff) + (ydiff * ydiff);

  if(int(dist_sq) > 1)
  {
    //ROS_WARN("New %f , %f Old %f , %f", msg.pose.position.x,msg.pose.position.y,prevx,prevy);
    ROS_ERROR("[Goal Publisher] Error in distance greater than 1m");
  }
  else
  {
    ROS_INFO("Received valid pose info");
  }
  prevx = this->goal_pose.pose.position.x;
  prevy = this->goal_pose.pose.position.x;
  return true;
}
