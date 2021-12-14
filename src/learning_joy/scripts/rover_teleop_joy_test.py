#!/usr/bin/env python
import rospy
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
	twist.linear.x = -3*data.axes[1]
	twist.angular.z = data.axes[0]
	pub_pos.publish(twist.angular.z)
	pub_vel.publish(twist.linear.x)
# Intializes everything
def start():
# publishing to "turtle1/cmd_vel" to control turtle1
	global pub_vel
	global pub_pos
	pub_pos = rospy.Publisher('/ROBOT/controller/position/steer_joint_1/command', Float64)
	pub_vel = rospy.Publisher('/ROBOT/controller/velocity/wheel_joint_1/command', Float64)
# subscribed to joystick inputs on topic "joy"
	rospy.Subscriber("joy", Joy, callback)
# starts the node
	rospy.init_node('teleop_rover')
	rospy.spin()

if __name__ == '__main__':
	start()
