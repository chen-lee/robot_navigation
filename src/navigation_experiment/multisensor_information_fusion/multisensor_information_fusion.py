#!/usr/bin/env python
# coding:utf-8
import rospy
import matplotlib.pyplot as plt
from get_coordinate.msg import position #导入位置消息
from nav_msgs.msg import Odometry#导入里程计消息
from sensor_msgs.msg import Imu#导入陀螺仪数据
from multisensor_information_fusion.msg import pos  #发布滤波后的消息
import numpy as np

posx = 0
posy = 0
dx = 0
dpsi = 0

def callback(data):
	global posx
	global posy
	posx = data.positionX/1000.0
	posy = data.positionY/1000.0
	#rospy.loginfo(str(x) + "   " + str(y))

def callback_odom(data):
	global dx
	dx = data.twist.twist.linear.x

def callback_imu(data):
	global dpsi
	dpsi = data.angular_velocity.z

def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("position", position,callback)
	rospy.Subscriber("/odom",Odometry,callback_odom)
	rospy.Subscriber("/mobile_base/sensors/imu_data",Imu,callback_imu)
	pub = rospy.Publisher("pos",pos)
	
	#kalman滤波的参数设置
	numstates = 5 #状态变量数量x,y,角度，速度，角速度
	dt = 1.0/50.0  #采样时间
	#初始化P0，如何初始化？
	P = np.diag([0.21,0.20,0.32,0.0003,0.003])
	#过程噪声协方矩阵Q
	sUWB = 0.5*0.2*dt**2 #假设0.2为最大的加速度
	sCourse = 0.1*dt
	sVelocity = 0.2*dt
	sYaw = 0.83*dt
	Q = np.diag([sUWB**2,sUWB**2,sCourse**2,sVelocity**2,sYaw**2])
	#测量方程H
	#测量噪声协方差R，怎么确定的？
	varUWB = 0.5
	varspeed = 0.1#1.0
	varyaw = 0.2
	R = np.matrix([[varUWB**2,0.0,0.0,0.0],
							[0.0,varUWB**2,0.0,0.0],
							[0.0,0.0,varspeed**2,0.0],
							[0.0,0.0,0.0,varyaw**2]])
	I = np.eye(numstates)
	#初始状态
	x = np.matrix([[float(posx),float(posy),0.0,float(dx),float(dpsi)]]).T
	print(x,x.shape)
	rospy.loginfo("hah")
	
	rate = 50
	r = rospy.Rate(rate)
	#f = open("data3.txt","w+")
	while(1):
		if np.abs(dpsi) < 0.001:#机器人向前走
			x[0] = x[0] + x[3]*dt * np.cos(x[2])
			x[1] = x[1] + x[3]*dt * np.sin(x[2])
			x[2] = x[2]
			x[3] = x[3]
			x[4] = 0.0000001#避免在雅可比矩阵的数值计算问题
		else:
			x[0] = x[0] + (x[3]/x[4]) * (np.sin(x[4]*dt+x[2]) - np.sin(x[2]))
			x[1] = x[1] + (x[3]/x[4]) * (-np.cos(x[4]*dt+x[2]) + np.cos(x[2]))
			x[2] = (x[2] + x[4]*dt) % (2.0*np.pi)
			x[3] = x[3]
			x[4] = x[4]
		a13 = float((x[3]/x[4]) * (np.cos(x[4]*dt+x[2]) - np.cos(x[2])))
		a14 = float((1.0/x[4]) * (np.sin(x[4]*dt+x[2]) - np.sin(x[2])))
		a15 = float((dt*x[3]/x[4])*np.cos(x[4]*dt+x[2]) - (x[3]/x[4]**2)*(np.sin(x[4]*dt+x[2]) - np.sin(x[2])))
		a23 = float((x[3]/x[4]) * (np.sin(x[4]*dt+x[2]) - np.sin(x[2])))
		a24 = float((1.0/x[4]) * (-np.cos(x[4]*dt+x[2]) + np.cos(x[2])))
		a25 = float((dt*x[3]/x[4])*np.sin(x[4]*dt+x[2]) - (x[3]/x[4]**2)*(-np.cos(x[4]*dt+x[2]) + np.cos(x[2])))
		JA = np.matrix([[1.0, 0.0, a13, a14, a15],
								[0.0,1.0, a23, a24, a25],
								[0.0, 0.0, 1.0, 0.0, dt],
								[0.0, 0.0, 0.0, 1.0, 0.0],
								[0.0, 0.0, 0.0, 0.0, 1.0]])
		P = JA*P*JA.T + Q
		hx = np.matrix([[float(x[0])],
								[float(x[1])],
								[float(x[3])],
								[float(x[4])]])
		JH = np.matrix([[1,0.0,0.0,0.0,0.0],
								[0.0,1.0,0.0,0.0,0.0],
								[0.0,0.0,0.0,1.0,0.0],
								[0.0,0.0,0.0,0.0,1.0]])
		S = JH*P*JH.T + R
		K = (P*JH.T) * np.linalg.inv(S)
		Z = np.matrix([[posx,posy,dx,dpsi]]).T
		y = Z - (hx)
		x = x + (K*y)
		P = (I - (K*JH))*P
		x_final = float(x[0])
		y_final = float(x[1])
		psi_final = float(x[2])
		dx_final = float(x[3])
		dpsi_final = float(x[4])
		
		posi = pos()
		
		posi.x = x_final
		posi.y = y_final
		posi.psi = psi_final
		posi.dx = dx_final
		posi.dpsi= dpsi_final
		pub.publish(posi)
		
		
		rospy.loginfo(str(x_final)+" "+str(y_final)+"  "+str(psi_final)+"  "+str(dx_final)+"  "+str(dpsi_final))
		#f.write(str(posx)+"\t"+str(posy)+"\t"+str(dx)+"\t"+str(dpsi)+"\t"+str(x_final)+"\t"+str(y_final)+"\t"+str(psi_final)+"\t"+str(dx_final)+"\t"+str(dpsi_final)+"\n")
		#f.write(str(x_final)+"\t"+str(y_final)+"\t"+str(psi_final)+"\t"+str(dx_final)+"\t"+str(dpsi_final)+"\n")
		#f.write(str(x)+"\t"+str(y)+"\t"+str(dx)+"\t"+str(dpsi)+"\n")
		
		r.sleep()


	rospy.spin()
	f.close()
if __name__ == '__main__':
	listener()
	#rospy.loginfo("hah")
