#! /usr/bin/env python
# coding:utf-8
import numpy as np
import matplotlib.pyplot as plt 
import matplotlib as mpl
import math



xorigal = []
yorigal = []
posx = []
posy = []
dx = []
dpsi = []

#读取txt文件
#name = [[],[],[],[]]
name = [[],[],[],[],[],[],[]]
with open('data20-17Good.txt') as f:#with open('data/data3_fangxing.txt') as f:
    for line in f:
        for each in enumerate(line.split()):
            name[each[0]].append(float(each[1]))
xorigal,yorigal,dx ,dpsi,xfinal,yfinal,psifinal= name

a = 1.02
b = 2.63
c = 1.96
d = 3.68 

#UWB测量值与实际值
##第一步  计算测量值与实际值误差，分段计算，分为四段
###第一段，右边的竖线
fig=plt.figure(figsize=(5,5))
left = 0
right = 1605
erroEach = []
xsec=xorigal[left:right]
ysec=yorigal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in xsec:
	erroEach.append(abs(line - b))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第二段，上边的线
fig=plt.figure(figsize=(5,5))
left = 1605
right = 2405

xsec=xorigal[left:right]
ysec=yorigal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in ysec:
	erroEach.append(abs(line - d))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第三段
fig=plt.figure(figsize=(5,5))
left = 2405
right = 3305

xsec=xorigal[left:right]
ysec=yorigal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in xsec:
	erroEach.append(abs(line - a))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第四段
fig=plt.figure(figsize=(5,5))
left = 3305
right = 4305

xsec=xorigal[left:right]
ysec=yorigal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in ysec:
	erroEach.append(abs(line - c))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

print(max(erroEach))
print(sum(erroEach)/float(len(erroEach)))


erroEach=[]

#plt.show()



fig=plt.figure(figsize=(5,5))
plt.scatter(xorigal,yorigal,s=50,marker='+',label='UWB Measured')
a = 1.02
b = 2.63
c = 1.96
d = 3.68 

BaseStationX=[0,4.37,0,0]
BaseStationY=[0,0,5.59,0]

trueLinex=[a,b,b,a,a]
trueLiney=[c,c,d,d,c]
plt.plot(trueLinex,trueLiney,label='True trajectory',c='r',lw=5)
#plt.scatter(BaseStationX,BaseStationY,s=100,label='Base stations',c='k',marker='o')
plt.xlabel('x [m]')
plt.ylabel('y [m]')
plt.xlabel('x [m]',fontsize=25)
plt.ylabel('y [m]', fontsize=25)
plt.grid(True)
plt.xlim(0,3.5)
plt.ylim(1,4.5)
plt.legend()




#里程计陀螺仪
ds = []
odomx=[]
odomy=[]
odompsi=0
oddx =2.63
oddy = 1.96
fig=plt.figure(figsize=(5,5))
for index in range(len(dx)):
	ds=0.02 * dx[index]#获得距离
	odompsi+=0.02 * dpsi[index]
	oddx += ds * math.cos(odompsi)
	oddy += ds * math.sin(odompsi)
	odomx.append(oddx)
	odomy.append(oddy)
plt.scatter(odomx,odomy,s=50,marker='+',label='Encode+Gyroscope')
trueLinex=[a,b,b,a,a]
trueLiney=[c,c,d,d,c]
plt.plot(trueLinex,trueLiney,label='True trajectory',c='r',lw=5)
plt.xlabel('x [m]',fontsize=25)
plt.ylabel('y [m]', fontsize=25)
plt.xlabel('x [m]')
plt.ylabel('y [m]')
plt.xlim(0,3.5)
plt.ylim(1,4.5)
plt.grid(True)
plt.legend()

#滤波结果
fig = plt.figure(figsize=(5,5))
plt.scatter(xfinal,yfinal,s=50,marker='+',label='EKF position estimation')
plt.plot(trueLinex,trueLiney,label='True trajectory',c='r',lw=5)
plt.xlabel('x [m]',fontsize=25)
plt.ylabel('y [m]', fontsize=25)
plt.xlim(0,3.5)
plt.ylim(1,4.5)

mpl.rcParams['font.size'] = 25   # 更改默认更新字体大小  
plt.grid(True)
plt.legend()

#EKF估计值与实际值
##第一步  计算测量值与实际值误差，分段计算，分为四段
###第一段，右边的竖线
fig=plt.figure(figsize=(5,5))
left = 0
right = 1605
erroEach = []
xsec=xfinal[left:right]
ysec=yfinal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in xsec:
	erroEach.append(abs(line - b))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第二段，上边的线
fig=plt.figure(figsize=(5,5))
left = 1605
right = 2405

xsec=xfinal[left:right]
ysec=yfinal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in ysec:
	erroEach.append(abs(line - d))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第三段
fig=plt.figure(figsize=(5,5))
left = 2405
right = 3305

xsec=xfinal[left:right]
ysec=yfinal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in xsec:
	erroEach.append(abs(line - a))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

###第四段
fig=plt.figure(figsize=(5,5))
left = 3305
right = 4305

xsec=xfinal[left:right]
ysec=yfinal[left:right]

plt.scatter(xsec,ysec,s=50,c='k',marker='+')

for line in ysec:
	erroEach.append(abs(line - c))

plt.xlim(0,3.5)
plt.ylim(1,4.5)

print(max(erroEach))
print(sum(erroEach)/float(len(erroEach)))


erroEach=[]

#机器人偏行角
fig=plt.figure(figsize=(5,5))




plt.show()    

#print Py

#Extended Kalman Filter



