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
xorigal1,yorigal1,dx1 ,dpsi1,xfinal1,yfinal1,psifinal1= name
startx = 400
endx = 3900
xorigal = xorigal1[startx:endx]
yorigal = yorigal1[startx:endx]
dx = dx1[startx:endx]
dpsi = dpsi1[startx:endx]
xfinal = xfinal1[startx:endx]
yfinal = yfinal1[startx:endx]
psifinal = psifinal1[startx:endx]

a = 1.02
b = 2.63
c = 1.96
d = 3.68 

#UWB测量值与实际值
##第一步  计算测量值与实际值误差，分段计算，分为四段
###第一段，右边的竖线
#fig=plt.figure(figsize=(5,5))
#left = 0
#right = 1605
#erroEach = []
#xsec=xorigal[left:right]
#ysec=yorigal[left:right]

#plt.scatter(xsec,ysec,s=50,c='k',marker='+')

#for line in xsec:
	#erroEach.append(abs(line - b))

#plt.xlim(0,3.5)
#plt.ylim(1,4.5)

####第二段，上边的线
#fig=plt.figure(figsize=(5,5))
#left = 1605
#right = 2405

#xsec=xorigal[left:right]
#ysec=yorigal[left:right]

#plt.scatter(xsec,ysec,s=50,c='k',marker='+')

#for line in ysec:
	#erroEach.append(abs(line - d))

#plt.xlim(0,3.5)
#plt.ylim(1,4.5)

####第三段
#fig=plt.figure(figsize=(5,5))
#left = 2405
#right = 3305

#xsec=xorigal[left:right]
#ysec=yorigal[left:right]

#plt.scatter(xsec,ysec,s=50,c='k',marker='+')

#for line in xsec:
	#erroEach.append(abs(line - a))

#plt.xlim(0,3.5)
#plt.ylim(1,4.5)

####第四段
#fig=plt.figure(figsize=(5,5))
#left = 3305
#right = 4305

#xsec=xorigal[left:right]
#ysec=yorigal[left:right]

#plt.scatter(xsec,ysec,s=50,c='k',marker='+')

#for line in ysec:
	#erroEach.append(abs(line - c))

#plt.xlim(0,3.5)
#plt.ylim(1,4.5)

#print(max(erroEach))
#print(sum(erroEach)/float(len(erroEach)))


#erroEach=[]

#plt.show()




a = 1.02
b = 2.63
c = 1.96
d = 3.68 






#里程计陀螺仪
ds = []
odomx=[]
odomy=[]
odompsi=0
oddx =2.63
oddy = 1.96

opsi=[]
for index in range(len(dx)):
	ds=0.02 * dx[index]#获得距离
	odompsi+=0.02 * dpsi[index]
	opsi.append(odompsi)
	oddx += ds * math.cos(odompsi)
	oddy += ds * math.sin(odompsi)
	odomx.append(oddx)
	odomy.append(oddy)






#机器人偏行角
tReal=[0,2.1889,4.21981,20.3249,22.8187,36.3214,39.1449,54.2169,57.6247,70]
tReal1=[57.6247,70]
psiReal=[0,0,np.pi/float(2),np.pi/float(2),np.pi,np.pi,3*np.pi/float(2),3*np.pi/float(2),2*np.pi,2*np.pi]
psiReal1=[0,0]

#滤波算法
fig=plt.figure(figsize=(5,5))
t=[]
for index in range(len(psifinal)):
	t.append(index*0.02)
plt.scatter(t,psifinal,s=50,marker='+',label='EKF yaw estimation')
plt.plot(tReal,psiReal,label='True yaw',c='r',lw=5)
plt.xlabel('t [s]',fontsize=25)
plt.ylabel('yaw [rad]', fontsize=25)
group_labels = ['0', '$0.5\pi$','$\pi$','1.5$\pi$','2$\pi$']  
grouoy = [0,0.5*np.pi,np.pi,1.5*np.pi,2*np.pi]
plt.yticks(grouoy,group_labels,rotation = 0)
plt.xlim(0,75)
plt.ylim(-1,8)
plt.grid(True)
plt.legend()
##分四段
#第一段
left = 212
right = 1015
erroEach = []
psisec=psifinal[left:right]
for line in psisec:
	erroEach.append(abs(line - 0.5*np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第二段
left = 1142
right = 1815
erroEach = []
psisec=psifinal[left:right]
for line in psisec:
	erroEach.append(abs(line - np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第三段
left = 1959
right = 2709
erroEach = []
psisec=psifinal[left:right]
for line in psisec:
	erroEach.append(abs(line - 1.5*np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第四段
left = 2883
right = 3500
erroEach = []
psisec=psifinal[left:right]
for line in psisec:
	if line > np.pi:
		erroEach.append(abs(line - 2*np.pi))
	else:
		erroEach.append(abs(line))
print(sum(erroEach)/float(len(erroEach)))



#陀螺仪测量得到的
fig=plt.figure(figsize=(5,5))
t=[]
for index in range(len(psifinal)):
	t.append(index*0.02)
plt.scatter(t,opsi,s=50,marker='+',label='Gyroscope Measured')
plt.plot(tReal,psiReal,label='True yaw',c='r',lw=5)
#plt.plot(tReal1,psiReal1,label='True yaw',c='r',lw=5)
plt.hold(True)
group_labels = ['0', '$0.5\pi$','$\pi$','1.5$\pi$','2$\pi$']  
grouoy = [0,0.5*np.pi,np.pi,1.5*np.pi,2*np.pi]
plt.yticks(grouoy,group_labels,rotation = 0)
plt.xlabel('t [s]',fontsize=25)
plt.ylabel('yaw [rad]', fontsize=25)
#plt.hold(True)
plt.xlim(0,75)
plt.ylim(-1,8)
plt.grid(True)
plt.legend()
##分四段
#第一段
left = 212
right = 1015
erroEach = []
psisec=opsi[left:right]
for line in psisec:
	erroEach.append(abs(line - 0.5*np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第二段
left = 1142
right = 1815
erroEach = []
psisec=opsi[left:right]
for line in psisec:
	erroEach.append(abs(line - np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第三段
left = 1959
right = 2709
erroEach = []
psisec=opsi[left:right]
for line in psisec:
	erroEach.append(abs(line - 1.5*np.pi))
print(sum(erroEach)/float(len(erroEach)))
#第四段
left = 2883
right = 3500
erroEach = []
psisec=opsi[left:right]
for line in psisec:
	if line > np.pi:
		erroEach.append(abs(line - 2*np.pi))
	else:
		erroEach.append(abs(line))
print(sum(erroEach)/float(len(erroEach)))



mpl.rcParams['font.size'] = 25   # 更改默认更新字体大小  

plt.show()    

#print Py

#Extended Kalman Filter



