#include <sstream>
#include <ros/console.h>
#include <RMC_1_hardware_interface/RMC_1_hardware_interface.h>
#include "odrive_cpp_lib/odrive.h"
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
    dhr::odrive od;
    Json::Value json;
    RMC1HardwareInterface::RMC1HardwareInterface(ros::NodeHandle& nh) : 
    nh_(nh)
     {
    //RMC1HardwareInterface::RMC1HardwareInterface(ros::NodeHandle& nh) : nh_(nh) {

    	od.init(0x2075378E5753); 
        dhr::getJson(&od, &json);
	init();
        controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));
        nh_.param("/ROBOT/hardware_interface/loop_hz", loop_hz_, 0.1);
        ros::Duration update_freq = ros::Duration(1.0/loop_hz_);
        motor_count = nh_.advertise<std_msgs::Float64>("motor_count",1);
        sub_vl = nh_.subscribe("/abs_pos_vl",1,&RMC1HardwareInterface::read_abs_pos_vl,this);
        sub_vr = nh_.subscribe("/abs_pos_vr",1,&RMC1HardwareInterface::read_abs_pos_vr,this);
        sub_hl = nh_.subscribe("/abs_pos_hl",1,&RMC1HardwareInterface::read_abs_pos_hl,this);
        sub_hr = nh_.subscribe("/abs_pos_hr",1,&RMC1HardwareInterface::read_abs_pos_hr,this);
        non_realtime_loop_ = nh_.createTimer(update_freq, &RMC1HardwareInterface::update, this);
	ros::Publisher command_input_pub = nh_.advertise<std_msgs::Float64>("command_input_publisher",1);
    }

    RMC1HardwareInterface::~RMC1HardwareInterface() {

    }

    void RMC1HardwareInterface::init() {
	//odrive initi beginn
	//dhr::odrive od;
           
        //Json::Value json;
       
        uint32_t state1 = 6, state2 = 8;
        //float vel = 7.0, vel1 = 0,vel_es=0,pos_es=0;
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
            JointHandle jointVelocityHandle(jointStateHandle, &joint_velocity_command_[i]);
            //JointLimits limits;
            //SoftJointLimits softLimits;
            //getJointLimits(joint_names_[i], nh_, limits);
            //PositionJointSoftLimitsHandle jointLimitsHandle(jointPositionHandle, limits, softLimits);
            //positionJointSoftLimitsInterface.registerHandle(jointLimitsHandle);
            position_joint_interface_.registerHandle(jointPositionHandle);
            velocity_joint_interface_.registerHandle(jointVelocityHandle);

            // Create effort joint interface
            JointHandle jointEffortHandle(jointStateHandle, &joint_effort_command_[i]);
            effort_joint_interface_.registerHandle(jointEffortHandle);
        }

        registerInterface(&joint_state_interface_);
        registerInterface(&position_joint_interface_);
        //registerInterface(&effort_joint_interface_);
        registerInterface(&positionJointSoftLimitsInterface);
        registerInterface(&velocity_joint_interface_);

	ROS_INFO("command: %s", "Hardware interface ready initialized");
	//odrive initi beginn
	dhr::writeOdriveData(&od, json, "axis1.requested_state", state1);
        sleep(6);
        dhr::writeOdriveData(&od, json, "axis0.requested_state", state1);
        sleep(5);
        dhr::writeOdriveData(&od, json, "axis1.requested_state", state2);
        sleep(1);
      	dhr::writeOdriveData(&od, json, "axis0.requested_state", state2);
	sleep(1);
        ROS_INFO("Gotov sam da machkam!");
        
        uint64_t stateread;
	dhr::readOdriveData(&od, json, "serial_number",stateread);
	std::cout << stateread << std::endl;
	
    }

    void RMC1HardwareInterface::update(const ros::TimerEvent& e) {
        elapsed_time_ = ros::Duration(e.current_real - e.last_real);
        read();
        controller_manager_->update(ros::Time::now(), elapsed_time_);
        write(elapsed_time_);
    }

    void RMC1HardwareInterface::read() {
   	float readvalue_pos,readvalue_vel,voltage;
   	std_msgs::Float64 msg;
       // for (int i = 0; i < num_joints_; i++) {
            //dhr::readOdriveData(&od, json, "axis0.encoder.vel_estimate",vel_es);
            dhr::readOdriveData(&od, json, "axis1.encoder.pos_estimate",readvalue_pos);
            msg.data = readvalue_pos;
            motor_count.publish(msg);
            joint_position_[0] = abs_pos_vl*3.14*2;
            //joint_position_[0] = readvalue_pos;
            ROS_INFO("Abs Pos %f",abs_pos_vl);
            sleep(0.001);
            dhr::readOdriveData(&od, json, "axis0.encoder.pos_estimate",readvalue_vel);
            joint_position_[1] = readvalue_vel*3.14*2/6.67;
            ROS_INFO("Pos Mot %f",readvalue_vel);
            sleep(0.001);
            dhr::readOdriveData(&od, json, "vbus_voltage",voltage);
            ROS_INFO("Battery Voltage: %f V", voltage);
            //Expanding preparation for 4 wheel system
            joint_position_[2] = abs_pos_vr*3.14*2; 
            joint_position_[3] = 0; 
            joint_position_[4] = abs_pos_hl*3.14*2; 
            joint_position_[5] = 0; 
            joint_position_[6] = abs_pos_hr*3.14*2;
            joint_position_[7] = 0;
           //ROBOT.getJoint(joint_names_[i]).read(); replace with actual code to get position from the controller
        //float steering_command_sent_= joint_position_command_[0]*steering_transmission_ratio_;
	//}
	
    }

    void RMC1HardwareInterface::write(ros::Duration elapsed_time) {
    	float pos=0,vel=0;
        //positionJointSoftLimitsInterface.enforceLimits(elapsed_time);
       // for (int i = 0; i < num_joints_; i++) {
        pos=joint_position_command_[0];
        vel=joint_velocity_command_[1];
        //ROS_INFO("command: %f",joint_position_command_[0] );
        dhr::writeOdriveData(&od, json, "axis1.controller.input_pos", pos);
        sleep(0.001);
        dhr::writeOdriveData(&od, json, "axis0.controller.input_vel", vel);
	//ROS_INFO("command: %f",joint_velocity_command_[1] );
        //    int j = i;//dummy ROBOT.getJoint(joint_names_[i]).actuate(joint_position_command_[i]); Replace with actual code to give commands to the controller
        //}
    }
    
       void RMC1HardwareInterface::read_abs_pos_vl(const std_msgs::Float64::ConstPtr& msg_vl) {
	abs_pos_vl=msg_vl->data; } 
	
	void RMC1HardwareInterface::read_abs_pos_vr(const std_msgs::Float64::ConstPtr& msg_vr) {
	abs_pos_vr=msg_vr->data; }
	
       void RMC1HardwareInterface::read_abs_pos_hl(const std_msgs::Float64::ConstPtr& msg_hl) {
	abs_pos_hl=msg_hl->data; }    
    
       void RMC1HardwareInterface::read_abs_pos_hr(const std_msgs::Float64::ConstPtr& msg_hr) {
	abs_pos_hr=msg_hr->data; }    
    
}
