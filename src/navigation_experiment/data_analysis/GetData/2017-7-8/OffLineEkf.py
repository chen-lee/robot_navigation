#! /usr/bin/env python
# coding:utf-8
import numpy as np
import matplotlib.pyplot as plt 

#系统的状态
numstates = 5 #状态变量数量x,y,角度，速度，角速度
dt = 1.0/50.0

#初始化P0，如何初始化？
P = np.diag([0.21,0.20,0.32,0.0003,0.003])#P = np.diag([100.0,100.0,100.0,100.0,100.0])

#过程噪声协方矩阵Q
sUWB = 0.5*0.2*dt**2 #假设0.2为最大的加速度
sCourse = 0.1*dt
sVelocity = 0.2*dt
sYaw = 0.83*dt
Q = np.diag([sUWB**2,sUWB**2,sCourse**2,sVelocity**2,sYaw**2])

#测量方程H
#测量噪声协方差R，怎么确定的？
varUWB = 2#1#5#5#20
varspeed = 0.001#2#0.1#1.0
varyaw = 0.001#0.2#0.01#0.2
R = np.matrix([[varUWB**2,0.0,0.0,0.0],
							[0.0,varUWB**2,0.0,0.0],
							[0.0,0.0,varspeed**2,0.0],
							[0.0,0.0,0.0,varyaw**2]])
print(R,R.shape)
#单位矩阵
I = np.eye(numstates)

xorigal = []
yorigal = []
posx = []
posy = []
dx = []
dpsi = []

#读取txt文件
#name = [[],[],[],[]]
name = [[],[],[],[],[],[],[]]
with open('2017-7-8-ST-Shape.txt') as f:#with open('data/data3_fangxing.txt') as f:
    for line in f:
        for each in enumerate(line.split()):
            name[each[0]].append(float(each[1]))
xorigal,yorigal,dx ,dpsi,a1,a2,a3= name
posx = [ix/1.0 for ix in xorigal]
posy = [iy/1.0 for iy in yorigal]


print len(posx)
print len(posy)
print max(dx)
print max(dpsi)


fig=plt.figure(figsize=(5,5))
plt.title('UWB Measurements')
plt.scatter(posx,posy,s=50,marker='+')


#初始状态
x = np.matrix([[posx[0],posy[0],0,dx[0],dpsi[0]]]).T
print(x,x.shape)

#测量矩阵
measurements = np.vstack((posx,posy,dx,dpsi))
m = measurements.shape[1]
print m
print(measurements.shape)

dstate=[]
x0=[]
x1=[]
#进入大循环
yaw = 0
x_odom = 0
xodom = []
y_odom = 0
yodom = []
Px = []
Py = []
Pdx = []
Pdy = []
Pddx = []
for filterstep in range(m):
	
	yaw = dpsi[filterstep] * dt + yaw
	x_odom = x_odom + dx[filterstep] * dt * np.cos(yaw)
	y_odom = y_odom + dx[filterstep] * dt * np.sin(yaw)
	xodom.append(x_odom)
	yodom.append(y_odom)
	
	if np.abs(dpsi[filterstep]) < 0.001:#Driving straight
		x[0] = x[0] + x[3]*dt * np.cos(x[2])
		x[1] = x[1] + x[3]*dt * np.sin(x[2])
		x[2] = x[2]
		x[3] = x[3]
		x[4] = 0.0000001#避免在雅可比矩阵的数值计算问题
		dstate.append(0)
	else:
		x[0] = x[0] + (x[3]/x[4]) * (np.sin(x[4]*dt+x[2]) - np.sin(x[2]))
		x[1] = x[1] + (x[3]/x[4]) * (-np.cos(x[4]*dt+x[2]) + np.cos(x[2]))
		x[2] = (x[2] + x[4]*dt) % (2.0*np.pi) #(x[2] + x[4]*dt + np.pi) % (2.0*np.pi) - np.pi对于角度大于360度时进行处理
		x[3] = x[3]
		x[4] = x[4]
		dstate.append(1)

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
	
	Z = measurements[:,filterstep].reshape(JH.shape[0],1)
	#print Z
	y = Z - (hx)
	x = x + (K*y)
	
	P = (I - (K*JH))*P
	x0.append(float(x[0]))
	x1.append(float(x[1]))
	Px.append(float(P[0,0]))
	Py.append(float(P[1,1]))
	Pdx.append(float(P[2,2]))
	Pdy.append(float(P[3,3]))
	Pddx.append(float(P[4,4]))


    
fig=plt.figure(figsize=(5,5))
plt.scatter(posx,posy,s=50,  label='UWB Measure',marker='+')
plt.scatter(x0,x1,  label='EKF Position')
plt.legend()

fig=plt.figure(figsize=(5,5))
plt.title('EKF')
plt.scatter(x0,x1)
 

fig=plt.figure(figsize=(5,5))
plt.title('encoder+gyroscope')
plt.scatter(xodom,yodom)
 


fig=plt.figure(figsize=(5,5))
plt.title('realtime')
plt.scatter(a1,a2)


plt.show()    

#print Py

#Extended Kalman Filter



