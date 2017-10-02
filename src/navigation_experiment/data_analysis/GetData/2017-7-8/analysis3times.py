#! /usr/bin/env python
# coding:utf-8
import numpy as np
import matplotlib.pyplot as plt 
import matplotlib as mpl
import math



#读取txt文件
xorigal1 = []
yorigal1= []
posx1 = []
posy1 = []
dx1 = []
dpsi1 = []
name = [[],[],[],[],[],[],[]]
with open('2017-7-8-1block.txt') as f:#with open('data/data3_fangxing.txt') as f:
    for line in f:
        for each in enumerate(line.split()):
            name[each[0]].append(float(each[1]))
xorigal1,yorigal1,dx1 ,dpsi1,xfinal1,yfinal1,psifinal1= name
f.close()
xorigal4 = []
yorigal4= []
posx4 = []
posy4 = []
dx4 = []
dpsi4 = []
name = [[],[],[],[],[],[],[]]
with open('2017-7-8-4block.txt') as f:#with open('data/data3_fangxing.txt') as f:
    for line in f:
        for each in enumerate(line.split()):
            name[each[0]].append(float(each[1]))
xorigal4,yorigal4,dx4 ,dpsi4,xfinal4,yfinal4,psifinal4= name
f.close()
xorigal9 = []
yorigal9= []
posx9 = []
posy9 = []
dx9 = []
dpsi9 = []
name = [[],[],[],[],[],[],[]]
with open('2017-7-8-9block.txt') as f:#with open('data/data3_fangxing.txt') as f:
    for line in f:
        for each in enumerate(line.split()):
            name[each[0]].append(float(each[1]))
xorigal9,yorigal9,dx9 ,dpsi9,xfinal9,yfinal9,psifinal9= name
f.close()



a = 1.02
b = 2.63
c = 1.96
d = 3.68 

fig=plt.figure(figsize=(5,5))
plt.scatter(xorigal1,yorigal1,s=50,marker='+',label='UWB Measured')
plt.scatter(xorigal4,yorigal4,s=50,marker='+')
plt.scatter(xorigal9,yorigal9,s=50,marker='+')
a1 = 1.83
b1 = 2.63
c1 = 1.96
d1 = 2.74

a4 = 1.40
b4 = 3.05
c4 = 1.51
d4 = 3.16 

a9 = 1.02
b9 = 3.43
c9 = 1.04
d9 = 3.68 


trueLinex1=[a1,b1,b1,a1,a1]
trueLiney1=[c1,c1,d1,d1,c1]

trueLinex4=[a4,b4,b4,a4,a4]
trueLiney4=[c4,c4,d4,d4,c4]

trueLinex9=[a9,b9,b9,a9,a9]
trueLiney9=[c9,c9,d9,d9,c9]
plt.plot(trueLinex1,trueLiney1,label='True trajectory1',c='r',lw=5)
plt.plot(trueLinex4,trueLiney4,label='True trajectory2',c='m',lw=5)
plt.plot(trueLinex9,trueLiney9,label='True trajectory3',c='y',lw=5)

plt.xlabel('x [m]')
plt.ylabel('y [m]')
plt.xlabel('x [m]',fontsize=25)
plt.ylabel('y [m]', fontsize=25)
plt.grid(True)
plt.xlim(0.25,4.25)
plt.ylim(0.5,4.5)
plt.legend()

plt.show()

