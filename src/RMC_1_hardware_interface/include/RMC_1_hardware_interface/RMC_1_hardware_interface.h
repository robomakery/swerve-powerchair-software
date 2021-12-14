#ifndef ROS_CONTROL__ROBOT_HARDWARE_INTERFACE_H
#define ROS_CONTROL__ROBOT_HARDWARE_INTERFACE_H

#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
#include <controller_manager/controller_manager.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>
//#include <ROBOTcpp/ROBOT.h>
#include <RMC_1_hardware_interface/RMC_1_hardware.h>
#include "odrive_cpp_lib/odrive.h"
#include "std_msgs/Float64.h"

using namespace hardware_interface;
using joint_limits_interface::JointLimits;
using joint_limits_interface::SoftJointLimits;
using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::PositionJointSoftLimitsInterface;


namespace RMC_1_hardware_interface
{
    static const double POSITION_STEP_FACTOR = 10;
    static const double VELOCITY_STEP_FACTOR = 10;

    class RMC1HardwareInterface: public RMC_1_hardware_interface::RMC1Hardware
    {
        public:
            RMC1HardwareInterface(ros::NodeHandle& nh);
            ~RMC1HardwareInterface();
            void init();
            void update(const ros::TimerEvent& e);
            void read();
            void write(ros::Duration elapsed_time);
            void read_abs_pos_vl(const std_msgs::Float64::ConstPtr& msg_vl);
            void read_abs_pos_vr(const std_msgs::Float64::ConstPtr& msg_vr);
            void read_abs_pos_hl(const std_msgs::Float64::ConstPtr& msg_hl);
            void read_abs_pos_hr(const std_msgs::Float64::ConstPtr& msg_hr);

        protected:
            //ROBOTcpp::ROBOT ROBOT;
            dhr::odrive od;
            Json::Value json;
            ros::NodeHandle nh_;
            ros::Timer non_realtime_loop_;
            ros::Duration control_period_;
            ros::Duration elapsed_time_;
            ros::Publisher motor_count;
            ros::Subscriber sub_vl, sub_vr, sub_hl, sub_hr;
            PositionJointInterface positionJointInterface;
            PositionJointSoftLimitsInterface positionJointSoftLimitsInterface;
            VelocityJointInterface velocityJointInterface;
            double loop_hz_;
            boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
            double p_error_, v_error_, e_error_;
      	    double steering_transmission_ratio_;
	    double drive_transmission_ratio_;
	    float abs_pos_vr, abs_pos_vl, abs_pos_hl, abs_pos_hr ;
	    std_msgs::Float64 msg_vl,msg_vr,msg_hl,msg_hr;
    };

}

#endif
