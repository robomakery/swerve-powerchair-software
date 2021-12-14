#!/usr/bin/env python
import rospy
import math
from std_msgs.msg import Float64
from geometry_msgs.msg import Twist 
from sensor_msgs.msg import Joy

# Author: Andrew Dai
# This ROS Node converts Joystick inputs from the joy node
# into commands for turtlesim

# Receives joystick messages (subscribed to Joy topic)
# then converts the joysick inputs into Twist commands
# axis 1 aka left stick vertical controls linear speed
# axis 0 aka left stick horizonal controls angular speed
def callback(data):
	twist = Twist()
	twist.linear.x = 2*data.axes[1]
	twist.linear.y = 2*data.axes[0]
	twist.angular.z = -2*math.pi*data.axes[3]/5
	pub_transl_x.publish(twist)

#	pub_vel_vl.publish(V_SP_y_command)
#	pub_pos_vr.publish(W_SP_rot_command)

# Intializes everything
def start():
# publishing to "turtle1/cmd_vel" to control turtle1
	global pub_transl_x
#	global pub_transl_y
#	global pub_ang_z
	
	
	
	pub_transl_x = rospy.Publisher('/swerve_steering_controller/cmd_vel',Twist,queue_size=3)
	rospy.init_node('teleop_rover_swerve_steering_controller',anonymous=True)
#	pub_transl_y = rospy.Publisher('/swerve_steering_controller/cmd_vel/linear', Float64)
#	pub_ang_z = rospy.Publisher('/swerve_steering_controller/cmd_vel/', Float64)


	rate = rospy.Rate(10)
# subscribed to joystick inputs on topic "joy"
	rospy.Subscriber("joy", Joy, callback)
# starts the node
	rate.sleep()
	rospy.spin()

if __name__ == '__main__':
	start()
