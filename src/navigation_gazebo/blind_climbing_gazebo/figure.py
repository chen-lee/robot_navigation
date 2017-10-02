#!/usr/bin/env python
# coding:utf-8
import rospy
import matplotlib.pyplot as plt
from sensor_msgs.msg import NavSatFix #导入消息

x = 0
y = 0

def callback(data):
	global x
	global y
	x = (data.latitude - 49.9) * 111000
	y = -(data.longitude - 8.9) * 71950
	rospy.loginfo(str(x) + "   " + str(y))

def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("/sensor_msgs/NavSatFix", NavSatFix,callback)
	while(1):
		#rospy.loginfo("hah")
		plt.scatter(x, y,marker = 'o', color = 'g', label='position', s = 10)
		plt.xlim(0,10)
		plt.ylim(-5,5)
		plt.pause(0.01)
		plt.hold(True)
	rospy.spin()
if __name__ == '__main__':
	listener()
	#rospy.loginfo("hah")
