# Install script for directory: /home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/dhrengineering/catkin_new_wheelchir_module/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/dhrengineering/catkin_new_wheelchir_module/build/ROS_Module_Assembly_v8/catkin_generated/installspace/ROS_Module_Assembly_v8.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8/cmake" TYPE FILE FILES
    "/home/dhrengineering/catkin_new_wheelchir_module/build/ROS_Module_Assembly_v8/catkin_generated/installspace/ROS_Module_Assembly_v8Config.cmake"
    "/home/dhrengineering/catkin_new_wheelchir_module/build/ROS_Module_Assembly_v8/catkin_generated/installspace/ROS_Module_Assembly_v8Config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8" TYPE FILE FILES "/home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8/config" TYPE DIRECTORY FILES "/home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8/config/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8/launch" TYPE DIRECTORY FILES "/home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8/launch/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8/meshes" TYPE DIRECTORY FILES "/home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8/meshes/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ROS_Module_Assembly_v8/urdf" TYPE DIRECTORY FILES "/home/dhrengineering/catkin_new_wheelchir_module/src/ROS_Module_Assembly_v8/urdf/")
endif()

