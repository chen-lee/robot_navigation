#!/usr/bin/env python
# coding:utf-8
import rospy
import matplotlib.pyplot as plt
import numpy as np
from ekf.msg import pos #导入消息

x = 0
y = 0
psi = 0

def callback(data):
	global x
	global y
	global psi
	x = data.x
	y = data.y
	psi = data.psi
	rospy.loginfo(str(x) + "   " + str(y) + "   "+str(psi*180.0/3.1415926))

def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("pos", pos,callback)
	while(1):
		#rospy.loginfo("hah")
		U = float(np.cos(psi)*0.5)
		V= float(np.sin(psi)*0.5)
		plt.quiver(x,y,U,V)
		plt.scatter(x, y,marker = 'o', color = 'g', label='position', s = 40)
		plt.xlim(-1,4)
		plt.ylim(-1,7)
		plt.pause(0.01)
		plt.hold(True)
	rospy.spin()
if __name__ == '__main__':
	listener()
	#rospy.loginfo("hah")
