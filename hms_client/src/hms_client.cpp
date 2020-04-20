#include <ros/ros.h>
#include <hms_client/hms_msg.h>
#include <hms_client/ping_pong.h>
#include <sensor_msgs/PointCloud2.h>
#include <bits/stdc++.h> 
#include <cstdlib>
#include<cctype>
#include<string>
using namespace std;

void PointCloud2_callback(const sensor_msgs::PointCloud2& msg)
{
   ROS_INFO("I heard sensor: [%d]", msg.is_dense);
   //sleep(10);
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "hms_client");
  ros::NodeHandle nh;
  ros::Subscriber sub;
  vector<ros::ServiceClient> clients;

  int num_nodes;
  string str, nodei;
  vector<string> nodes;
  ros::param::get("num_nodes", num_nodes);
  cout << num_nodes;
  for(int i = 0; i < num_nodes; i++)
  {
    string hc = "health_check_";
    str = "node" + to_string(i + 1);
    ros::param::get(str, nodei);
    cout << nodei.c_str(); 
    nodes.push_back(nodei.c_str());
    hc.append(nodei);
    clients.push_back(nh.serviceClient<hms_client::ping_pong>(hc));

  }

  int num_topics;
  string topici, str_;
  ros::param::get("num_topics", num_topics);
  cout << num_topics;
  for(int i = 0; i < num_topics; i++)
  {
    str_ = "topic" + to_string(i + 1);
    ros::param::get(str_, topici);
    //cout << topici.c_str(); 
    string top = "dummy";
    if(topici.c_str() == top)
      sub = nh.subscribe(topici.c_str(), 1, PointCloud2_callback);
  }
  ros::Rate loop_rate(20);
  //ros::MultiThreadedSpinner spinner(2);
  while(ros::ok()) {

    for(int j = 0; j < num_nodes; j++)
    {
      hms_client::ping_pong srv;
      srv.request.node_name = nodes[j];
      bool val = 0;
      for(int k = 0; k < 3 && val == 0; k++)
        val = clients[j].call(srv);
      ROS_INFO("%d", val);
      if(val == 0)
      {
        ROS_INFO("Error code: %ld, from %s", (long int)srv.response.error_code, srv.request.node_name.c_str());
        ROS_ERROR("!!!Failed to call service, node malfunctioning!!!");
      }
    }
    //spinner.spin();
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  return 0;
}