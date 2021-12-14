#include <sstream>
#include "std_msgs/Float64.h"
#include <ros/console.h>
#include <RMC_1_hardware_interface/RMC_1_hardware_interface.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
//#include <ROBOTcpp/ROBOT.h>
using namespace hardware_interface;
using joint_limits_interface::JointLimits;
using joint_limits_interface::SoftJointLimits;
using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::PositionJointSoftLimitsInterface;

namespace RMC_1_hardware_interface
{

    RMC1HardwareInterface::RMC1HardwareInterface(ros::NodeHandle& nh) : nh_(nh) {
        init();
        controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));
        nh_.param("/ROBOT/hardware_interface/loop_hz", loop_hz_, 0.1);
        ros::Duration update_freq = ros::Duration(1.0/loop_hz_);
        non_realtime_loop_ = nh_.createTimer(update_freq, &RMC1HardwareInterface::update, this);
    
	ros::Publisher command_input_pub = nh_.advertise<std_msgs::Float64>("command_input_publisher",1000);
    }

    RMC1HardwareInterface::~RMC1HardwareInterface() {

    }

    void RMC1HardwareInterface::init() {
	// Get joint names
        nh_.getParam("/ROBOT/hardware_interface/joints", joint_names_);
        num_joints_ = joint_names_.size();

	nh_.getParam("/steering_transmission_ratio",steering_transmission_ratio_);
	ROS_INFO("Transmission ratio %f", steering_transmission_ratio_);

        // Resize vectors
        joint_position_.resize(num_joints_);
        joint_velocity_.resize(num_joints_);
        joint_effort_.resize(num_joints_);
        joint_position_command_.resize(num_joints_);
        joint_velocity_command_.resize(num_joints_);
        joint_effort_command_.resize(num_joints_);

        // Initialize Controller 
        for (int i = 0; i < num_joints_; ++i) {
            //ROBOTcpp::Joint joint = ROBOT.getJoint(joint_names_[i]); print joint_names_ and replace that row with actual controller initialization code
		std::cout << "PENIS!!!";
             // Create joint state interface
            JointStateHandle jointStateHandle(joint_names_[i], &joint_position_[i], &joint_velocity_[i], &joint_effort_[i]);
            joint_state_interface_.registerHandle(jointStateHandle);

            // Create position joint interface
            JointHandle jointPositionHandle(jointStateHandle, &joint_position_command_[i]);
            //JointLimits limits;
            //SoftJointLimits softLimits;
            //getJointLimits(joint_names_[i], nh_, limits);
            //PositionJointSoftLimitsHandle jointLimitsHandle(jointPositionHandle, limits, softLimits);
            //positionJointSoftLimitsInterface.registerHandle(jointLimitsHandle);
            position_joint_interface_.registerHandle(jointPositionHandle);

            // Create effort joint interface
            JointHandle jointEffortHandle(jointStateHandle, &joint_effort_command_[i]);
            effort_joint_interface_.registerHandle(jointEffortHandle);
        }

        registerInterface(&joint_state_interface_);
        registerInterface(&position_joint_interface_);
        //registerInterface(&effort_joint_interface_);
        registerInterface(&positionJointSoftLimitsInterface);

	ROS_INFO("command: %s", "Hardware interface ready initialized");
    }

    void RMC1HardwareInterface::update(const ros::TimerEvent& e) {
        elapsed_time_ = ros::Duration(e.current_real - e.last_real);
        read();
        controller_manager_->update(ros::Time::now(), elapsed_time_);
        write(elapsed_time_);
	ROS_INFO("command: %f",joint_position_command_[0] );
    }

    void RMC1HardwareInterface::read() {
        for (int i = 0; i < num_joints_; i++) {
            joint_position_[i] = 3.14;//ROBOT.getJoint(joint_names_[i]).read(); replace with actual code to get position from the controller
        float steering_command_sent_= joint_position_command_[0]*steering_transmission_ratio_;
	}
    }

    void RMC1HardwareInterface::write(ros::Duration elapsed_time) {
        //positionJointSoftLimitsInterface.enforceLimits(elapsed_time);
        for (int i = 0; i < num_joints_; i++) {
            int j = i;//dummy ROBOT.getJoint(joint_names_[i]).actuate(joint_position_command_[i]); Replace with actual code to give commands to the controller
        }
    }
}
