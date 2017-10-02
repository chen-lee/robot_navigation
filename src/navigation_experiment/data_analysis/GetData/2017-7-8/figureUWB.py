#!/usr/bin/env python
# coding:utf-8
import rospy
import matplotlib.pyplot as plt
from readserial2st.msg import position #导入消息

x = 0
y = 0

def callback(data):
	global x
	global y
	x = data.positionX
	y = data.positionY
	rospy.loginfo(str(x) + "   " + str(y))

def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("position", position,callback)
	while(1):
		#rospy.loginfo("hah")
		plt.scatter(x, y,marker = 'o', color = 'g', label='position', s = 40)
		plt.xlim(0,5000)
		plt.ylim(1000,6000)
		plt.pause(0.01)
		plt.hold(True)
	rospy.spin()
if __name__ == '__main__':
	listener()
	#rospy.loginfo("hah")
