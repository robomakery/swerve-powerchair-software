#include <RMC_1_hardware_interface/RMC_1_hardware_interface.h>
#include <ros/callback_queue.h>
#include "odrive_cpp_lib/odrive.h"

int main(int argc, char** argv){

	    
	ros::init(argc, argv, "RMC_1_hardware_interface");
    	ros::CallbackQueue ros_queue;

    	ros::NodeHandle nh;
    	       
    	nh.setCallbackQueue(&ros_queue);
        
    
    	RMC_1_hardware_interface::RMC1HardwareInterface rhi(nh);
    
	
    	ROS_INFO("Hello %s", "End");
    	ros::MultiThreadedSpinner spinner(0);
    	spinner.spin(&ros_queue);

    return 0;
}
