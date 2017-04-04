#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <vector>

sensor_msgs::LaserScan laser_scan; 
float min_range;

void scanCallBack(const sensor_msgs::LaserScan::ConstPtr& msg) {
	std::vector<float> laser;
	laser = msg->ranges;
	
	int laser_size = laser.size();
	for (int i=0;i<laser_size;i++){
		if (laser[i] < msg->range_min){
			laser[i] = 99999;
		}
		if (laser[i] > msg->range_max){
			laser[i] = 99999;
		}
	}

	min_range = laser[0];
	int min_index;
	for (int i=0; i<laser_size; i++){
		if (laser[i]<min_range){
			min_range = laser[i];
			min_index = i; //non neccessario
			ROS_INFO("Minimum distance read is %f",min_range);
		} 
	}
	
	for (int j=0;j<laser_size;j++){
		if (laser[j] > min_range + 0.5){
			laser[j] = 0;
		}
	}		
	
	laser_scan = *msg;
	laser_scan.ranges.clear();
	laser_scan.ranges = laser;
	
}

int main(int argc, char **argv) {
	
	ros::init(argc,argv,"distance_node");
	
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("base_scan",1000,scanCallBack);
	ros::Publisher pub = n.advertise<sensor_msgs::LaserScan>("closest_points",1000);
	
	ros::Rate loop_rate(10);
	
	while (ros::ok()) {
		
		pub.publish(laser_scan);
		
		ros::spinOnce();
		
		loop_rate.sleep();
		
	}
	
	return 0;

}
