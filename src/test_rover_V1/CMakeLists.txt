cmake_minimum_required(VERSION 2.8.3)

project(test_rover_V1)

find_package(catkin REQUIRED)

catkin_package()

find_package(catkin REQUIRED COMPONENTS
robot_state_publisher
rviz
joint_state_publisher_gui
urdf
xacro
)

foreach(dir config launch meshes urdf)
	install(DIRECTORY ${dir}/
		DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)
