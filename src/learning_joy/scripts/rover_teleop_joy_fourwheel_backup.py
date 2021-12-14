#!/usr/bin/env python
import rospy
import math
import numpy
from std_msgs.msg import Float64
from geometry_msgs.msg import Twist 
from sensor_msgs.msg import Joy
from nav_msgs.msg import Odometry
from tf.transformations import euler_from_quaternion, quaternion_from_euler

# Author: Andrew Dai
# This ROS Node converts Joystick inputs from the joy node
# into commands for turtlesim
w_last = [0, 0, 0, 0]
#Lenght in meters
L=12.8;
#Width in meters
W=8.8;
button_pressed=0;
button_pressed_gyro=0;
def getGyro(odom_data):
	global roll, pitch, yaw
	orientation_q=odom_data.pose.pose.orientation
#    global counter
#    rospy.sleep(1)
#    curr_time = odom_data.header.stamp
	orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
	(roll,pitch,yaw)=euler_from_quaternion(orientation_list)
	#pose = odom_data.pose.pose.orientation.w #  the x,y,z pose and quaternion orientation
#    counter= counter+1
#   print counter, curr_time
#   print
#    print pose
	pub_orientation.publish((yaw+math.pi)*180/math.pi)


# Receives joystick messages (subscribed to Joy topic)
# then converts the joysick inputs into Twist commands
# axis 1 aka left stick vertical controls linear speed
# axis 0 aka left stick horizonal controls angular speed
def callback(data):
	global w_last
	global L,W
	global button_pressed, button_pressed_gyro
	
	twist = Twist()
	twist.linear.x = data.axes[0];
	twist.linear.y = data.axes[1];
	twist.angular.z = data.axes[3];
	
	
	if(data.buttons[1] == 1):
	
		button_pressed= 1 - button_pressed
		rospy.sleep(1);
	
	if(data.buttons[0] == 1):

		button_pressed_gyro= 1 - button_pressed_gyro
		rospy.sleep(1);
	
	
	if (button_pressed == 0) and (button_pressed_gyro == 0):
		FWD_command = -twist.linear.y
		STR_command = -twist.linear.x
		W_SP_rot_command = twist.angular.z
	

	#Gyro
	if (button_pressed == 0) and (button_pressed_gyro == 1):
		temp = -twist.linear.y * math.cos(yaw) - twist.linear.x * math.sin(yaw);
		STR_command  = twist.linear.y*math.sin(yaw) - twist.linear.x*math.cos(yaw);
		FWD_command  = temp;
		W_SP_rot_command = twist.angular.z
	
	if (button_pressed == 1) :
	#
	#Ackermann
		#V_SP_x_command = -twist.linear.y
		#V_SP_y_command = twist.linear.x*numpy.sign(-twist.linear.y)
		#W_SP_rot_command = (twist.linear.x*numpy.sign(-twist.linear.y)*math.sqrt(L**2+W**2))/L;
		
		FWD_command = -twist.linear.y
		STR_command = -twist.angular.z*numpy.sign(twist.linear.y)
		W_SP_rot_command = -(STR_command*math.sqrt(L**2+W**2))/L;
	
	#pub_cmd.publish(V_SP_y_command)

#Help variable
	R=math.sqrt(L**2+W**2);
	A=STR_command-W_SP_rot_command*(L/R);
	B=STR_command+W_SP_rot_command*(L/R);	
	C=FWD_command-W_SP_rot_command*(W/R);
	D=FWD_command+W_SP_rot_command*(W/R);
	
	#Wheel speeds
	vfr=math.sqrt(B**2+C**2);
	vfl=math.sqrt(B**2+D**2);
	vrl=math.sqrt(A**2+D**2);
	vrr=math.sqrt(A**2+C**2);
	
	#Wheel oreient target 
	wfr=math.atan2(B,C)*180/math.pi;
	wfl=math.atan2(B,D)*180/math.pi;
	wrl=math.atan2(A,D)*180/math.pi;
	wrr=math.atan2(A,C)*180/math.pi;
	
	v=[vfr,vfl,vrl,vrr]
	w=[wfr,wfl,wrl,wrr]
	
	
	#Finding the least angle movement and eventuell inverting wheel speed direction
	for i in range(4):
	#inside for loop
		tagetAngle = w[i];
		tagetAngle %= 360;
		#offset need to be add
		
		currentAngle = w_last[i];
		currentAngleMod = currentAngle % 360;
		
		if currentAngleMod <0:
			currentAngleMod += 360;
		
		delta = currentAngleMod - tagetAngle;
	
		if (delta > 180):
			tagetAngle +=360
			
		elif (delta < -180):
			tagetAngle -= 180
			#Change direction ?
			v[i]=-v[i]
		
		delta = currentAngleMod - tagetAngle;
		
		if (delta > 90 or delta < -90): 
			if (delta > 90):
				tagetAngle += 180;
			elif (delta < -90):
				tagetAngle -= 180;
				v[i]=v[i]
		else:
			v[i]=-v[i]
		
		tagetAngle += currentAngle -currentAngleMod;
		w[i] = tagetAngle*math.pi/180;
	
	
	wfr=w[0]
	wfl=w[1]
	wrl=w[2]
	wrr=w[3]
	
	vfr=v[0]
	vfl=v[1]
	vrl=v[2]
	vrr=v[3]
	
	
	
	#%normalizing wheel speed
	lim=abs(vfr);
	
	if abs(vfl)>lim:
		lim=vfl;
	
	if abs(vrl)>lim:
		lim=vrl;
		
	if abs(vrr)>lim:
		lim=vrr;
	
	if lim>1:	
		vfr = vfr/lim;
		vfl = vfl/lim; 
		vrl = vrl/lim; 
		vrr = vrr/lim;

	pub_vel_vr.publish(40*vfr)
	pub_vel_vl.publish(40*vfl)
	pub_vel_hl.publish(40*vrl)
	pub_vel_hr.publish(40*vrr)
	
	
	if abs(twist.linear.x)>0.06 or abs(twist.linear.y)>0.06 or abs(twist.angular.z)>0.06:
		pub_pos_vr.publish(wfr);
		pub_pos_vl.publish(wfl);
		pub_pos_hl.publish(wrl);
		pub_pos_hr.publish(wrr);
	#Current Angle in simulation
	w_last = w	
	
	
	
# Intializes everything
def start():
# publishing to "turtle1/cmd_vel" to control turtle1
	global pub_vel_vl
	global pub_pos_vl
	global pub_vel_vr
	global pub_pos_vr
	global pub_vel_hl
	global pub_pos_hl
	global pub_vel_hr
	global pub_pos_hr
	global pub_orientation
	global pub_cmd
	
	pub_pos_vr = rospy.Publisher('/ROBOT/controller/position/steer_joint_1/command', Float64)
	pub_vel_vr = rospy.Publisher('/ROBOT/controller/velocity/wheel_joint_1/command', Float64)
	pub_pos_vl = rospy.Publisher('/ROBOT/controller/position/steer_joint_2/command', Float64)
	pub_vel_vl = rospy.Publisher('/ROBOT/controller/velocity/wheel_joint_2/command', Float64)
	pub_pos_hl = rospy.Publisher('/ROBOT/controller/position/steer_joint_3/command', Float64)
	pub_vel_hl = rospy.Publisher('/ROBOT/controller/velocity/wheel_joint_3/command', Float64)
	pub_pos_hr = rospy.Publisher('/ROBOT/controller/position/steer_joint_4/command', Float64)
	pub_vel_hr = rospy.Publisher('/ROBOT/controller/velocity/wheel_joint_4/command', Float64)
	pub_orientation = rospy.Publisher('/orient',Float64)
	pub_cmd = rospy.Publisher('V_SP_y_command', Float64)
# subscribed to joystick inputs on topic "joy"
	rospy.Subscriber("/joy", Joy, callback)
	rospy.Subscriber("/odomw",Odometry,getGyro)
	
# starts the node
	rospy.init_node('teleop_rover')
	rospy.spin()

if __name__ == '__main__':
	start()
