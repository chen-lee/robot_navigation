#!/usr/bin/env python
# coding:utf-8
import sys
import signal
import time
import rospy
import matplotlib.pyplot as plt
import numpy as np
from multisensor_information_fusion.msg import pose #导入消息

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

def quit(signum, frame):
    print ''
    print 'stop showing'
    sys.exit()

def show_traject():
    signal.signal(signal.SIGINT, quit)
    signal.signal(signal.SIGTERM, quit)
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("pose", pose,callback)
    while 1:
        U = float(np.cos(psi)*0.5)
        V= float(np.sin(psi)*0.5)
        plt.quiver(x,y,U,V)
        plt.scatter(x, y,marker = 'o', color = 'g', label='position', s = 40)
        plt.xlim(-1,8)
        plt.ylim(-1,8)
        plt.pause(0.01)
        plt.hold(True)
    #rospy.spin()

if __name__ == '__main__':
    show_traject()
