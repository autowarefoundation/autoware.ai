#include <string>
#include "ros/ros.h"
#include "dpm/ImageObjects.h"

extern int kf_main(int argc, char* argv[],
			const std::string& tracking_type);

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pedestrian_tracking");

	ros::NodeHandle n;
	std::string tracking_type("pedestrian");

	return kf_main(argc, argv, tracking_type);
}
