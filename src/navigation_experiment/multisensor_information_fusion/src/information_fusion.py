#!/usr/bin/env python
# coding:utf-8
import rospy
import matplotlib.pyplot as plt
from get_coordinate.msg import position #导入位置消息
from nav_msgs.msg import Odometry#导入里程计消息
from sensor_msgs.msg import Imu#导入陀螺仪数据
from multisensor_information_fusion.msg import pose  #发布滤波后的消息
import numpy as np

class Fusion(self):
    def __init__(self):
        #ros initial
        rospy.init_node('listener', anonymous=True)
        #subscriber sensor topic
        rospy.Subscriber("position", position,self.callback_position)
        rospy.Subscriber("/odom",Odometry,self.callback_odom)
        rospy.Subscriber("/mobile_base/sensors/imu_data",Imu,self.callback_imu)
        #publish result to pose
        pub = rospy.Publisher("pose",pose)
        #extended kalman filter parameter setting
        num_states = 5
        dt = 1.0/50.0

        self.P


    #callback function get position x , y
    def callback_position(self, data):
        self.raw_x = data.positionX / 1000.0
        self.raw_y = data.positionY / 1000.0

    #callback function get speed
    def callback_odom(self, data):
        self.raw_dx = data.twist.twist.linear.x
    
    #callback function get angle speed
    def callback_imu(self,data):
        self.raw_dpsi = data.angular_velocity.z

if __name__ == '__main__':
	
