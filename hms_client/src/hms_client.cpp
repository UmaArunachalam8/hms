#include <ros/ros.h>
#include <hms_client/hms_msg.h>
#include <hms_client/ping_pong.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <bits/stdc++.h> 
#include <cstdlib>
#include<cctype>
#include<string>
using namespace std;

int counter = 0;
void PointCloud2_callback(const sensor_msgs::PointCloud2& msg)
{
   if(msg.is_dense)
    ROS_INFO("I heard valid pcd data");
   else
    ROS_ERROR("I heard invalid pcd data");
   counter++;
   //sleep(10);
}
void LaserScan_callback(const sensor_msgs::LaserScan& msg)
{
   int min_ = msg.range_min - 0.1;
   int max_ = msg.range_max + 0.1;
   int flag = 1;
   // for(int h = 0; 560; h++)
   //  if(msg.ranges[h] > max_ || msg.ranges[h] < min_)
   //    flag = 0;
   if(!flag)
    ROS_ERROR("I heard invalid laser scan data");
   else 
    ROS_INFO("I heard valid laser scan data");
   counter++;
   //sleep(10);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "hms_client");
  ros::NodeHandle nh;
  vector<ros::Subscriber> subs;
  vector<ros::ServiceClient> clients;
  ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  int cb_queue = 1;
  geometry_msgs::Twist msg;

  int num_nodes;
  string str, nodei;
  vector<string> nodes;
  ros::param::get("num_nodes", num_nodes);
  for(int i = 0; i < num_nodes; i++)
  {
    string hc = "health_check_";
    str = "node" + to_string(i + 1);
    ros::param::get(str, nodei);
    //cout << nodei.c_str(); 
    nodes.push_back(nodei.c_str());
    hc.append(nodei);
    clients.push_back(nh.serviceClient<hms_client::ping_pong>(hc));

  }
  vector<string> topics;
  vector<float> rates;
  int num_topics;
  string topici, str_, r;
  float ri;
  ros::param::get("num_topics", num_topics);
  for(int i = 0; i < num_topics; i++)
  {
    str_ = "topic" + to_string(i + 1);
    r = "rate" + to_string(i + 1);
    ros::param::get(str_, topici);
    ros::param::get(r, ri);
    topics.push_back(topici.c_str());
    rates.push_back(ri);
    //cout << topici.c_str(); 
    switch(i)//////////////////////////////////////////////////////for now redundant///////////////////////////////////////
    {
      case 0: subs.push_back(nh.subscribe(topics[i], cb_queue, PointCloud2_callback));
              break;
      case 1: subs.push_back(nh.subscribe(topics[i], cb_queue, LaserScan_callback));
              break;
    }
  }
  float lr;
  ros::param::get("min_rate", lr);
  lr = min(lr, float(1));
  cout << lr;
  ros::Rate loop_rate(lr);
  //ros::MultiThreadedSpinner spinner(2);
  while(ros::ok()) {
    counter = 0;
    string nodem = "goal_publisher";
    ros::spinOnce();
    for(int j = 0; j < num_nodes; j++)
    {
      hms_client::ping_pong srv;
      srv.request.node_name = nodes[j];
      bool val = 0;
      for(int k = 0; k < 3 && val == 0; k++)
        val = clients[j].call(srv);
      if(val)
        ROS_INFO("Node %s is functioning properly", nodes[j].c_str());
      else
      {
        ROS_ERROR("Error code: %ld, from %s, !!!Failed to call service, node malfunctioning!!!", (long int)srv.response.error_code, srv.request.node_name.c_str());
        //ROS_ERROR("!!!Failed to call service, node malfunctioning!!!");
        if(nodes[j].c_str() == nodem)
        {
          geometry_msgs::Twist twist_msg;
          twist_msg.linear.x = 0;
          vel_pub.publish(twist_msg);
          ROS_ERROR("Node %s instructed to come to halt", nodes[j].c_str());


        }
      }
    }
    //spinner.spin();
    //cout << counter;
    float check = 0;
    for(auto r: rates)
      if(r > lr)
        check += (r / lr);
      else
        check += 1;
    
    //cout << counter << endl;
    if(cb_queue == 1)
      check = num_topics;
    if(counter == check)
      ROS_INFO("Incoming stream is steady, some data is received from all sensors");
    else
      ROS_ERROR("!!!Failed to recieve data, sensing sys malfunctioning!!!");

    loop_rate.sleep();
  }  
  return 0;
}